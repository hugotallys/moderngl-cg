extends Node3D

## How fast the camera rotates when dragging.
@export var sensitivity: float = 0.005

## The maximum angle (in degrees) you can look up or down.
@export var pitch_limit_degrees: float = 89.0

## How fast the camera zooms in and out.
@export var zoom_speed: float = 0.5
## The closest the camera can get to the origin.
@export var min_radius: float = .5
## The farthest the camera can get from the origin.
@export var max_radius: float = 10.0

# Get the child nodes we need
@onready var pitch_pivot: Node3D = $PitchPivot
@onready var camera: Camera3D = $PitchPivot/Camera3D

var _min_pitch: float
var _max_pitch: float


func _ready() -> void:
	# Calculate the pitch limits in radians (which Godot uses for rotation)
	_min_pitch = deg_to_rad(-pitch_limit_degrees)
	_max_pitch = deg_to_rad(pitch_limit_degrees)
	
	# Set the starting radius to be within the limits
	camera.position.z = clamp(camera.position.z, min_radius, max_radius)


func _input(event: InputEvent) -> void:
	
	# --- Handle Zooming ---
	if event is InputEventMouseButton:
		if event.is_pressed():
			if event.button_index == MOUSE_BUTTON_WHEEL_UP:
				camera.position.z -= zoom_speed
			elif event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
				camera.position.z += zoom_speed
				
			# Clamp the radius
			camera.position.z = clamp(camera.position.z, min_radius, max_radius)

	# --- Handle Orbiting (Drag) ---
	if event is InputEventMouseMotion:
		if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
			# Left/Right (Yaw)
			rotate_y(-event.relative.x * sensitivity)
			# Up/Down (Pitch)
			pitch_pivot.rotate_x(-event.relative.y * sensitivity)
			# Clamp the Pitch
			pitch_pivot.rotation.x = clamp(pitch_pivot.rotation.x, _min_pitch, _max_pitch)
