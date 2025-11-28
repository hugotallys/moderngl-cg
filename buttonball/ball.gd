extends RigidBody3D

## Adjust this in the Inspector to change how strong the movement force is.
@export var force_strength: float = .01


# Called every physics frame. 'delta' is the elapsed time since the previous frame.
# We use _physics_process for RigidBody nodes to keep physics calculations stable.
func _physics_process(delta: float) -> void:
	# 1. Get input from the arrow keys (or WASD by default)
	# This returns a 2D vector like (x, y) where values range from -1.0 to 1.0.
	var input_vector_2d = Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	
	# 2. Map the 2D input to a 3D force direction
	# We use the 2D vector's X for our 3D X-axis.
	# We use the 2D vector's Y for our 3D Z-axis (forward/backward).
	var force_direction = Vector3(input_vector_2d.x, 0, input_vector_2d.y)
	
	# 3. Apply the force
	# We multiply the direction by our force_strength.
	# 'apply_central_force' pushes the object.
	apply_central_force(force_direction * force_strength)
