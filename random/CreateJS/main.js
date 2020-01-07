MOUSE_BUTTON_LEFT_ID	= 0;
MOUSE_BUTTON_MIDDLE_ID	= 1;
MOUSE_BUTTON_RIGHT_ID	= 2;

SCREEN_SIZE = {width: 640, height: 400};
var context, queue, stage;
var doSomethingTimer;
var sound_status = 0;
var sound_Icon;

window.onload = function() {
	console.log("Click to see coordinate location");
	console.log("Right Click to end repeating process");
	console.log("Type something to see its KeyValue displayed");

	var canvas = document.getElementById("myCanvas");
	context = canvas.getContext("2d");
	context.canvas.width = SCREEN_SIZE.width;
	context.canvas.height = SCREEN_SIZE.height;
	stage = new createjs.Stage("myCanvas");
	stage.enableMouseOver(10);

	queue = new createjs.LoadQueue(false);
	queue.installPlugin(createjs.Sound);
	queue.on("complete", queueLoaded, this);

	queue.loadManifest("../build/manifest.json");
	queue.load();
}

function queueLoaded(event) {
	doSomething();
	doSomethingTimer = setInterval(doSomething, 1000);

	manage_sound();
	sound_Icon.on("click", function(event) {
		toggle_sound();
	});

	for(var y=0;y<2;y++) {
		for(var x=0;x<2;x++) {
			var tile = new createjs.Bitmap(queue.getResult("image01"));
			tile.x = tile.image.width * x;
			tile.y = tile.image.height * y;
			stage.addChildAt(tile, 0);
		}
	}

	var waveMan_SpriteSheet = new createjs.SpriteSheet({
		images: [queue.getResult("image02")],
		frames: {width: 100, height: 100},
		animations: {
			wave: {
				frames: [0, 1, 2, 1],
				speed: 0.5
			}
		}
	});

	var waveMan = new createjs.Sprite(waveMan_SpriteSheet, "wave");
	waveMan.regX = waveMan.getBounds().width / 2;
	waveMan.regY = waveMan.getBounds().height / 2;
	waveMan.x = SCREEN_SIZE.width / 2;
	waveMan.y = SCREEN_SIZE.height / 2;
	waveMan.gotoAndPlay("wave");
	stage.addChild(waveMan);
	waveMan.on("mouseover", function(event){console.log("Mouse is over waveMan");});

	createjs.Ticker.setFPS(60);
	createjs.Ticker.addEventListener("tick", stage);
	createjs.Ticker.addEventListener("tick", tickEvent);

	// Set up events AFTER the game is loaded
	window.onmousedown = handleMouseDown;
	window.onmousemove = handleMouseMove;
	window.onkeydown = handleKeyDown;
}

function toggle_sound() {
	if(sound_status == 0)
		sound_status = 1;
	else
		sound_status = 0;

	manage_sound();
}

function manage_sound() {
	stage.removeChild(sound_Icon);
	var name = "Sound_Off";
	if(sound_status == 0)
		var name = "Sound_On";

	if(sound_Icon == null)
		sound_Icon = new createjs.Bitmap(queue.getResult(name));
	else
		sound_Icon.image = queue.getResult(name);

	stage.addChild(sound_Icon);

	if(sound_status == 1)
		createjs.Sound.play("audio01", {loop: -1, volume: 1.0});
	else
		createjs.Sound.stop("audio01");
}

function tickEvent() {
	stage.update();
}

function handleMouseDown(event) {
	if(event.button == MOUSE_BUTTON_RIGHT_ID) {
		clearInterval(doSomethingTimer);
	}

	console.log("(" + event.clientX + ", " + event.clientY + ")");
}

function handleMouseMove(event) {
	//
}

function handleKeyDown(event) {
	var tmp = new createjs.Text(event.code, "72px Arial", "#FFFF00");
	tmp.x = (SCREEN_SIZE.width / 2) - (tmp.getBounds().width / 2);
	tmp.y = (SCREEN_SIZE.height / 2) - (tmp.getBounds().height / 2);
	stage.addChild(tmp);

	createjs.Tween.get(tmp).to({alpha: 0}, 500).call(function(event) {
		stage.removeChild(event.target);
	});
}

function doSomething() {
	var tmp = new createjs.Bitmap(queue.getResult("image03"));
	tmp.x = SCREEN_SIZE.width - tmp.image.width;
	stage.addChild(tmp);
	createjs.Tween.get(tmp).to({alpha: 0}, 500).call(function(event) {
		stage.removeChild(event.target);
	});
}