extends Area3D

# This defines a new signal that this node can send.
# We'll tell our main game script to "listen" for this signal.
signal goal_scored

# This function is built-in. It runs automatically when the
# Area3D's "body_entered" signal fires.
func _on_body_entered(body: Node3D) -> void:
	# Check if the body that entered is in the "ball" group
	if body.is_in_group("ball"):
		# If it is, emit our custom "goal_scored" signal.
		goal_scored.emit()
