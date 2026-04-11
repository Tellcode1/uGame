struct Object {
  let name;
  let position;
  let scale;
  let rotation;
  let accum;
};

  //  Called once to initialize the object
  //  The engine doesn't have anything else to do with
  //  the object after this, only passes it to update.
fn init() {
  let obj = Object("Test", vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 0.0);
  return obj;
}

  // 
  // 
  //  The update function.
  //  Called as many times per second as possible
  //  Return the new state of the object on completion
  //  Keep it as fast as possible!
  // 
  //
fn update(obj, dt) {
  if (obj.name != "Test" || obj == null) { io::println(io::STDERR, "null object passed!"); return null; }
  
  const speed = 1.5;

  // obj.position.x = obj.position[0] + speed * dt;
  obj.position.x = 3 * math::sin(obj.accum);
  obj.accum += dt * speed;

  // io::println(io::STDOUT, "New position: ", obj.position.x + 10 * dt);

  return obj;
}

  //
  // 
  // The tick function.
  // Called 60 times a second. Always.
  // If the program is failing to keep up with
  // fixed updates, it will most definitely lag out
  // of existence, which means the player has worse
  // problems to deal with... (Possible crash, or worse)
  // 
  // 
fn tick(obj)
{
	return obj;
}
