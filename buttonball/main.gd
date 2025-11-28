extends Node3D

# --- Score Variables ---
var score_team_1 = 0
var score_team_2 = 0

# --- Node References ---
# Get the nodes we need to talk to.
# ($ScoreLabel assumes it's a direct child, adjust path if needed)
@onready var score_label: Label3D = $ScoreLabel
@onready var ball: RigidBody3D = $Ball # Adjust path to your ball
@onready var goal_area_1 = $GoalArea_TeamA # Adjust path
@onready var goal_area_2 = $GoalArea_TeamB
@onready var whistle_sound: AudioStreamPlayer3D = $WhistleSound
@onready var autoriza_sound: AudioStreamPlayer3D = $AutorizaSound
@onready var torcida_sound: AudioStreamPlayer3D = $TorcidaSound

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# 1. Connect the goal_scored signal from the Area3D
	#    to a function *in this script*.
	goal_area_1.goal_scored.connect(_on_team_1_scored)
	goal_area_2.goal_scored.connect(_on_team_2_scored)
	
	# (If you have a second goal, you'd connect it here too)
	# goal_area_2.goal_scored.connect(_on_team_2_scored)
	
	torcida_sound.play()
	autoriza_sound.play()
	
	# 2. Set the initial score text
	update_score_display()


# This function runs when GoalArea_Team1 emits "goal_scored"
func _on_team_1_scored() -> void:
	score_team_1 += 1
	update_score_display()
	
	whistle_sound.play()
	
	# IMPORTANT: Reset the ball so you can score again!
	# You'll want to reset its position and stop its movement.
	ball.global_position = Vector3(0.0, 0.2, 0.) # Or center of the field
	ball.linear_velocity = Vector3.ZERO
	ball.angular_velocity = Vector3.ZERO

# This function runs when GoalArea_Team1 emits "goal_scored"
func _on_team_2_scored() -> void:
	score_team_2 += 1
	update_score_display()
	
	whistle_sound.play()
	
	# IMPORTANT: Reset the ball so you can score again!
	# You'll want to reset its position and stop its movement.
	ball.global_position = Vector3(0.0, 0.2, 0.) # Or center of the field
	ball.linear_velocity = Vector3.ZERO
	ball.angular_velocity = Vector3.ZERO

# A helper function to update the text
func update_score_display() -> void:
	score_label.text = "%d - %d" % [score_team_1, score_team_2]
