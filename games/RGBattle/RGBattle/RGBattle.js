function init() {
	var	b2Vec2 = Box2D.Common.Math.b2Vec2;
	var b2AABB = Box2D.Collision.b2AABB;
	var b2BodyDef = Box2D.Dynamics.b2BodyDef;
	var b2Body = Box2D.Dynamics.b2Body;
	var b2FixtureDef = Box2D.Dynamics.b2FixtureDef;
	var b2Fixture = Box2D.Dynamics.b2Fixture;
	var b2World = Box2D.Dynamics.b2World;
	var b2MassData = Box2D.Collision.Shapes.b2MassData;
	var b2PolygonShape = Box2D.Collision.Shapes.b2PolygonShape;
	var b2CircleShape = Box2D.Collision.Shapes.b2CircleShape;
	var b2DebugDraw = Box2D.Dynamics.b2DebugDraw;
	var b2MouseJointDef = Box2D.Dynamics.Joints.b2MouseJointDef;

	var world = new b2World(new b2Vec2(0, 0), true);	// Gravity, Allow Sleep

	var fixDef = new b2FixtureDef;
	fixDef.density = 1.0;
	fixDef.friction = 0.5;
	fixDef.restitution = 0.2;

	var bodyDef = new b2BodyDef;

	// Create ground
	bodyDef.type = b2Body.b2_staticBody;
	fixDef.shape = new b2PolygonShape;
	fixDef.shape.SetAsBox(20, 2);
	bodyDef.position.Set(10, 400 / 30 + 1.8);
	world.CreateBody(bodyDef).CreateFixture(fixDef);
	bodyDef.position.Set(10, -1.8);
	world.CreateBody(bodyDef).CreateFixture(fixDef);
	fixDef.shape.SetAsBox(2, 14);
	bodyDef.position.Set(-1.8, 13);
	world.CreateBody(bodyDef).CreateFixture(fixDef);
	bodyDef.position.Set(21.8, 13);
	world.CreateBody(bodyDef).CreateFixture(fixDef);

	////
	fixDef.shape.SetAsBox(2, 14);
	bodyDef.position.Set(10, 400 / 30 + 1.8);
	world.CreateBody(bodyDef).CreateFixture(fixDef);
	////

	// Create square
	bodyDef.type = b2Body.b2_dynamicBody;
	fixDef.shape = new b2PolygonShape;
	fixDef.shape.SetAsBox(1.0, 1.0);
	bodyDef.position.x = 5.0;
	bodyDef.position.y = 5.0;
	bodyDef.fixedRotation = true;
	world.CreateBody(bodyDef).CreateFixture(fixDef);

	////
	bodyDef.type = b2Body.b2_dynamicBody;
	fixDef.shape = new b2PolygonShape;
	fixDef.shape.SetAsBox(1.0, 1.0);
	bodyDef.position.x = 10.0;
	bodyDef.position.y = 5.0;
	bodyDef.fixedRotation = true;
	world.CreateBody(bodyDef).CreateFixture(fixDef);
	////

	// Setup debug draw
	var debugDraw = new b2DebugDraw();
	debugDraw.SetSprite(document.getElementById("canvas").getContext("2d"));
	debugDraw.SetDrawScale(30.0);
	debugDraw.SetFillAlpha(0.5);
	debugDraw.SetLineThickness(1.0);
	debugDraw.SetFlags(b2DebugDraw.e_shapeBit | b2DebugDraw.e_jointBit);
	world.SetDebugDraw(debugDraw);

	window.setInterval(update, 1000 / 60);

	// Mouse

	var mouseX, mouseY, mousePVec, isMouseDown, mouseJoint;
	var selectedBody = null, selectedBodies = [], select_aabb = new b2AABB();
	var canvasPosition = getElementPosition(document.getElementById("canvas"));
	var isSelecting = false, selectStartX, selectStartY, selectEndX, selectEndY;
	var rallyingObjects = [];

	// Prevents the default menu from popping up
	window.oncontextmenu = function () {
		return false;
	}

	function handleMouseDown(e) {
		isMouseDown = true;

		if(e.button == 0) {
			selectStartX = (e.clientX - canvasPosition.x) / 30;
			selectStartY = (e.clientY - canvasPosition.y) / 30;
		}
		else if(e.button == 2) {
			var rightClickX = (e.clientX - canvasPosition.x) / 30;
			var rightClickY = (e.clientY - canvasPosition.y) / 30;
			for(var i=0;i<selectedBodies.length;i++) {
				removeFromRally(selectedBodies[i]);
				var posA = new b2Vec2(rightClickX, rightClickY);
				var posB = selectedBodies[i].GetPosition();
				var vector = new b2Vec2(posA.x - posB.x, posA.y - posB.y);
				vector.Normalize();
				vector.x *= 5.0;
				vector.y *= 5.0;
				selectedBodies[i].SetLinearVelocity(vector);
				selectedBodies[i].SetAwake(true);

				// Temp testing for velocity changes while moving
				selectedBodies[i].SetUserData(posA);
				addToRally(selectedBodies[i]);

				/*var md = new b2MouseJointDef();
				md.bodyA = world.GetGroundBody();
				md.bodyB = selectedBodies[i];
				md.target.Set(selectedBodies[i].GetPosition().x, selectedBodies[i].GetPosition().y);
				md.collideConnected = true;
				md.maxForce = 300.0;
				var joint = world.CreateJoint(md);
				joint.SetTarget(new b2Vec2(rightClickX, rightClickY));
				console.log(joint);
				selectedBodies[i].SetUserData(joint);
				selectedBodies[i].SetAwake(true);
				addToRally(selectedBodies[i]);*/
			}
		}

		handleMouseMove(e);

		document.addEventListener("mousemove", handleMouseMove, true);
		document.addEventListener("touchmove", handleMouseMove, true);
	}

	document.addEventListener("mousedown", handleMouseDown, true);
	document.addEventListener("touchstart", handleMouseDown, true);

	function handleMouseUp() {
		isMouseDown = false;
		mouseX = undefined;
		mouseY = undefined;

		if(isSelecting) {
			isSelecting = false;
			selectedBodies = getBodiesInSelection();
		}

		document.removeEventListener("mousemove", handleMouseMove, true);
		document.removeEventListener("touchmove", handleMouseMove, true);
	}

	document.addEventListener("mouseup", handleMouseUp, true);
	document.addEventListener("touchend", handleMouseUp, true);

	function handleMouseMove(e) {
		var clientX, clientY;
		if(e.clientX) {
			clientX = e.clientX;
			clientY = e.clientY;
		}
		else if(e.changedTouches && e.changedTouches.length > 0) {
			var touch = e.changedTouches[e.changedTouches.length - 1];
			clientX = touch.clientX;
			clientY = touch.clientY;
		}
		else {
			return;
		}
		mouseX = (clientX - canvasPosition.x) / 30;
		mouseY = (clientY - canvasPosition.y) / 30;
		e.preventDefault();

		if(e.button == 0) {
			selectEndX = (e.clientX - canvasPosition.x) / 30;
			selectEndY = (e.clientY - canvasPosition.y) / 30;

			if(isSelecting || selectStartX != selectEndX || selectStartY != selectEndY) {
				isSelecting = true;
			}
		}
	}

	function getBodyAtMouse() {
		mousePVec = new b2Vec2(mouseX, mouseY);
		var aabb = new b2AABB();
		aabb.lowerBound.Set(mouseX - 0.001, mouseY - 0.001);
		aabb.upperBound.Set(mouseX + 0.001, mouseY + 0.001);

		selectedBody = null;
		world.QueryAABB(getBodyCB, aabb);
		return selectedBody;
	}

	function getBodiesInSelection() {
		var correctStartX = Math.min(selectStartX, selectEndX);
		var correctStartY = Math.min(selectStartY, selectEndY);
		var correctEndX = Math.max(selectStartX, selectEndX);
		var correctEndY = Math.max(selectStartY, selectEndY);
		select_aabb.lowerBound.Set(correctStartX - 0.001, correctStartY - 0.001);
		select_aabb.upperBound.Set(correctEndX + 0.001, correctEndY + 0.001);

		selectedBodies = [];
		world.QueryAABB(getBodies, select_aabb);
		return selectedBodies;
	}

	function getBodyCB(fixture) {
		if(fixture.GetBody().GetType() != b2Body.b2_staticBody) {
			if(fixture.GetShape().TestPoint(fixture.GetBody().GetTransform(), mousePVec)) {
				selectedBody = fixture.GetBody();
				return false;
			}
		}
		return true;
	}

	function getBodies(fixture) {
		if(fixture.GetBody().GetType() != b2Body.b2_staticBody) {
			// Only select bodies that exist entirely within the selection box
			if(	fixture.GetAABB().lowerBound.x >= select_aabb.lowerBound.x &&
				fixture.GetAABB().lowerBound.y >= select_aabb.lowerBound.y &&
				fixture.GetAABB().upperBound.x <= select_aabb.upperBound.x &&
				fixture.GetAABB().upperBound.y <= select_aabb.upperBound.y) {
					selectedBodies.push(fixture.GetBody());
			}
		}
		return true;
	}

	function addToRally(body) {
		rallyingObjects.push(selectedBodies[i]);
	}

	function removeFromRally(body) {
		var index = rallyingObjects.indexOf(body);
		if(index > -1) {
			rallyingObjects.splice(index, 1);
		}
		body.SetUserData(null);
		body.SetLinearVelocity(new b2Vec2(0, 0));
	}

	function manageRallys() {
		//
	}

	function update() {
		/*if(isMouseDown && (!mouseJoint)) {
			var body = getBodyAtMouse();
			if(body) {
				var md = new b2MouseJointDef();
				md.bodyA = world.GetGroundBody();
				md.bodyB = body;
				md.target.Set(mouseX, mouseY);
				md.collideConnected = true;
				md.maxForce = 300.0 * body.GetMass();
				mouseJoint = world.CreateJoint(md);
				body.SetAwake(true);
			}
		}

		if(mouseJoint) {
			if(isMouseDown) {
				mouseJoint.SetTarget(new b2Vec2(mouseX, mouseY));
			}
			else {
				world.DestroyJoint(mouseJoint);
				mouseJoint = null;
			}
		}*/

		manageRallys();

		world.Step(1 / 60, 10, 10);
		world.DrawDebugData();
		world.ClearForces();

		if(isSelecting) {
			var c = document.getElementById("canvas");
			var ctx = c.getContext("2d");
			var canvasStartX = selectStartX * 30;
			var canvasStartY = selectStartY * 30;
			var rectWidth = (selectEndX - selectStartX) * 30;
			var rectHeight = (selectEndY - selectStartY) * 30;
			ctx.rect(canvasStartX, canvasStartY, rectWidth, rectHeight);
			ctx.strokeStyle = "red";
			ctx.stroke();
		}
	}

	// Helpers
	// http://js-tut.aardon.de/js-tut/tutorial/position.html
	function getElementPosition(element) {
		var elem = element, tagname = "", x = 0, y = 0;

		while((typeof(elem) == "object") && (typeof(elem.tagName) != "undefined")) {
			y += elem.offsetTop;
			x += elem.offsetLeft;
			tagname = elem.tagName.toUpperCase();

			if(tagname == "BODY") {
				elem = 0;
			}

			if(typeof(elem) == "object") {
				if(typeof(elem.offsetParent) == "object")
					elem = elem.offsetParent;
			}
		}
		return {x: x, y: y};
	}
}