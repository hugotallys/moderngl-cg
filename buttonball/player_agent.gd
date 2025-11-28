extends RigidBody3D

## Defines the team choices, which will appear as a dropdown in the Inspector.
enum Team { TEAM_A, TEAM_B }

## This is the exported variable. You can set this in the editor.
@export var team: Team = Team.TEAM_A

## How hard the player kicks the ball.
@export var kick_force: float = 0.1
## How long (in seconds) the player must wait after a kick.
@export var cooldown_time: float = 2.0

# --- Node References ---
@onready var detection_area: Area3D = $DetectionArea
@onready var cooldown_timer: Timer = $CooldownTimer

@onready var mesh: MeshInstance3D = $MeshInstance3D

# --- State Variables ---
# This holds the ball node when it's in range.
var target_ball: Node3D = null
# This is our simple state machine.
var is_on_cooldown: bool = false


func _ready() -> void:
	# Connect all the signals (as you did before)
	detection_area.body_entered.connect(_on_detection_area_body_entered)
	detection_area.body_exited.connect(_on_detection_area_body_exited)
	cooldown_timer.timeout.connect(_on_cooldown_timer_timeout)

	# --- NEW: Set Team Color ---
	# 1. Get the mesh's base material.
	var base_material = mesh.mesh.surface_get_material(0)

	# 2. If it doesn't have one, create a new one.
	if not base_material:
		base_material = StandardMaterial3D.new()

	# 3. CRITICAL: Make a unique copy (duplicate) for this instance.
	# This prevents all players from sharing the same color.
	var unique_material = base_material.duplicate()

	# 4. Set the color on the unique material based on the 'team' variable.
	if team == Team.TEAM_A:
		unique_material.albedo_color = Color.RED
	else: # Team.TEAM_B
		unique_material.albedo_color = Color.BLUE
		# You can also change the kick direction for the other team
		# For example, by multiplying kick_force by -1 if they
		# are on the other side of the field.

	# 5. Apply this new unique material to the mesh.
	mesh.set_surface_override_material(0, unique_material)


# Called every physics frame. This is our main logic loop.
func _physics_process(_delta: float) -> void:
	# 1. If we are on cooldown, do nothing.
	if is_on_cooldown:
		return
	
	# 2. If we have a target ball, kick it.
	if target_ball != null:
		kick()


func kick() -> void:
	# Safety check: make sure the ball still exists and we're not on cooldown.
	if not is_instance_valid(target_ball) or is_on_cooldown:
		return

	# --- Start Cooldown ---
	is_on_cooldown = true
	cooldown_timer.start(cooldown_time)

	# --- Calculate Kick Direction ---
	# 1. Get vector from us to the ball.
	var direction = (target_ball.global_position - global_position).normalized()
	# 2. IMPORTANT: Remove any Y-axis movement. We only kick on the XZ plane.
	direction.y = 0
	# 3. Re-normalize after zeroing Y to ensure full force.
	direction = direction.normalized()
	
	# --- Apply Kick Force ---
	# We use an "impulse" for a sudden, powerful kick.
	apply_central_impulse(direction * kick_force)


# --- Signal Functions ---

# This runs when something enters the "DetectionArea"
func _on_detection_area_body_entered(body: Node3D) -> void:
	# If the body is in the "ball" group, set it as our target.
	if body.is_in_group("ball"):
		target_ball = body


# This runs when something leaves the "DetectionArea"
func _on_detection_area_body_exited(body: Node3D) -> void:
	# If the body leaving is our target, clear it.
	if body == target_ball:
		target_ball = null


# This runs when the "CooldownTimer" finishes
func _on_cooldown_timer_timeout() -> void:
	# Cooldown is over. We are ready to kick again.
	is_on_cooldown = false
