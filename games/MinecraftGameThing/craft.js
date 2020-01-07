$(document).ready(function() {
	var canvas = $("#canvas")[0];
	var ctx = canvas.getContext("2d");
	ctx.imageSmoothingEnabled = false;
	var w = $("#canvas").width();
	var h = $("#canvas").height();

	var scale = 30;
	var camera = {x: w/2 / scale, y: h/2 / scale};
	var blockSize = 1;
	var inventorySlot = 0;

	function Inventory() {
		this.slots = [];
	}

	Inventory.prototype.addBlock = function(name) {
		var inventoryContainsBlock = false;
		for(var i in this.slots) {
			if(this.slots[i].block.name == name) {
				inventoryContainsBlock = true;
				this.slots[i].count++;
			}
		}

		if(!inventoryContainsBlock) {
			var block = new Block(name);
			var shouldAppend = true;

			for(var i=0;i<this.slots.length;i++) {
				if(this.slots[i] == undefined) {
					this.slots[i] = {block: block, count: 1};
					shouldAppend = false;
					break;
				}
			}

			if(shouldAppend) {
				this.slots.push({block: block, count: 1});
			}
		}
	};

	Inventory.prototype.removeBlock = function(name) {
		for(var i=0;i<this.slots.length;i++) {
			if(typeof this.slots[i] != "undefined" && this.slots[i].block.name == name) {
				this.slots[i].count--;
				break;
			}
		}

		for(var i=0;i<this.slots.length;i++) {
			if(typeof this.slots[i] != "undefined" && this.slots[i].count <= 0) {
				delete this.slots[i];
				break;
			}
		}
	};

	Inventory.prototype.blockAtIndex = function(index) {
		if(typeof this.slots[index] == "undefined") {
			return undefined;
		}
		else {
			return this.slots[index].block;
		}
	};

	function Block(name) {
		this.name = name;
		this.inventory = new Inventory();
		this.x = 0;
		this.y = 0;
		this.direction = "";
		this.interactDirection = "";
		this.wall = true;
		this.health = 100;
		this.fallen = false;

		this.icon = document.createElement("canvas");
		this.icon.width = this.icon.height = blockSize;
		this.ctx = this.icon.getContext("2d");
		this.ctx.fillStyle = "#00FFFF";
		if(this.name == "player")this.ctx.fillStyle = "#0000FF";
		if(this.name == "dirt")this.ctx.fillStyle = "#993300";
		if(this.name == "lava")this.ctx.fillStyle = "#FF0000";
		this.ctx.fillRect(0, 0, 1, 1);
	}

	Block.prototype.render = function() {
		ctx.save();
		ctx.scale(scale, scale);
		ctx.translate(camera.x-0.5, camera.y-0.5);
		var iconScale = 1.0;
		if(this.fallen)iconScale = 0.5;
		var iconScaleBuffering = this.icon.width * iconScale / 2;
		ctx.drawImage(this.icon, this.x - iconScaleBuffering, this.y - iconScaleBuffering, this.icon.width * iconScale, this.icon.height * iconScale);
		ctx.restore();
	};

	Block.prototype.affectHealth = function(delta) {
		if(this.fallen) {
			//
		}
		else {
			this.health += delta;
		}
	}

	Block.prototype.manageHealth = function() {
		if(this.health <= 0 && !this.fallen) {
			this.health = 100;
			this.fallen = true;
			this.wall = false;
		}

		if(!this.fallen) {
			this.health = Math.min(100, this.health+1);
		}
	};

	Block.prototype.move = function() {
		var canMove = true;

		var destinationX = this.x;
		var destinationY = this.y;
		if(false) {}
		else if(this.direction == "up")
			destinationY--;
		else if(this.direction == "down")
			destinationY++;
		else if(this.direction == "left")
			destinationX--;
		else if(this.direction == "right")
			destinationX++;
		else return;

		var collidedBlocks = blocksAtLocation(destinationX, destinationY);
		for(var i in collidedBlocks) {
			if(collidedBlocks[i] != this && collidedBlocks[i].wall == true) {
				canMove = false;
			}
		}

		if(canMove) {
			this.x = destinationX;
			this.y = destinationY;
		}

		this.direction = "";
	};

	Block.prototype.manageCollisions = function() {
		var toRemove = [];

		var collidedBlocks = blocksAtLocation(this.x, this.y);
		for(var i in collidedBlocks) {
			if(collidedBlocks[i] != this) {
				// Collisions for Player
				if(this == player) {
					if(collidedBlocks[i].fallen) {
						player.inventory.addBlock(collidedBlocks[i].name);
						toRemove.push(collidedBlocks[i]);
					}
				}
			}
		}

		for(var i in toRemove)
			removeBlock(toRemove[i]);
	}

	Block.prototype.interact = function() {
		var interactX = this.x;
		var interactY = this.y;

		if(false) {}
		else if(this.interactDirection == "up")
			interactY--;
		else if(this.interactDirection == "down")
			interactY++;
		else if(this.interactDirection == "left")
			interactX--;
		else if(this.interactDirection == "right")
			interactX++;
		else return;

		var shouldPlaceBlock = true;
		var interactBlocks = blocksAtLocation(interactX, interactY);
		for(var i in interactBlocks) {
			if(interactBlocks[i] != this) {
				interactBlocks[i].affectHealth(-35);
				shouldPlaceBlock = false;
			}
		}

		if(shouldPlaceBlock) {
			var blockToPlace = player.inventory.blockAtIndex(inventorySlot);
			if(blockToPlace != undefined) {
				var name = player.inventory.slots[inventorySlot].block.name;
				var block = new Block(name);
				block.x = interactX;
				block.y = interactY;
				player.inventory.removeBlock(block.name);
				blocks.push(block);
			}
		}

		this.interactDirection = "";
	};

	Block.prototype.tick = function() {
		if(this.name == "lava") {
			//
		}
	};

	blocks = [];

	var player = new Block("player");
	player.color = "blue";
	blocks.push(player);

	for(var i=0;i<10;i++) {
		var dirt = new Block("dirt");
		dirt.x = -5 + i;
		dirt.y = -2;
		blocks.push(dirt);
	}

	var lava = new Block("lava");
	lava.x = 2;
	lava.y = 2;
	blocks.push(lava);

	function blocksAtLocation(x, y) {
		retVal = [];

		for(var i in blocks) {
			if(blocks[i].x == x && blocks[i].y == y) {
				retVal.push(blocks[i]);
			}
		}

		return retVal;
	}

	function removeBlock(block) {
		for(var i=0;i<blocks.length;i++) {
			if(blocks[i] == block) {
				blocks.splice(i, 1);
				break;
			}
		}
	}

	function init() {
		score = 0;

		if(typeof game_loop != "undefined")
			clearInterval(game_loop);
		game_loop = setInterval(gameLoop, 60);
	}
	init();

	function gameLoop() {
		for(var i in blocks) {
			blocks[i].move();
			blocks[i].interact();
			blocks[i].manageHealth();
			blocks[i].manageCollisions();
			blocks[i].tick();
		}
		render();
	}

	function render() {
		ctx.fillStyle = "#EEEEEE";
		ctx.fillRect(0, 0, w, h);
		ctx.strokeStyle = "black";
		ctx.lineWidth = 1;
		ctx.strokeRect(0, 0, w, h);

		for(var i in blocks) {
			blocks[i].render();
		}

		ctx.fillStyle = "blue";
		ctx.fillText("Score: " + score, 5, 15);

		renderHotbar();
	}

	function renderHotbar() {
		var widthBuffer = w - w*0.9;
		var hotbarBlockSize = (w - widthBuffer) / 10;
		var iconSize = hotbarBlockSize * 0.5;
		for(var i=0;i<10;i++) {
			if(i == inventorySlot)ctx.lineWidth = 5;
			else ctx.lineWidth = 1;
			var x = widthBuffer/2 + hotbarBlockSize*i;
			var y = h-10 - hotbarBlockSize;
			ctx.strokeStyle = "black";
			ctx.strokeRect(x, y, hotbarBlockSize, hotbarBlockSize);

			var block = player.inventory.blockAtIndex(i);
			if(block != undefined) {
				icon = block.icon;
				iconX = x + hotbarBlockSize/2 - iconSize/2;
				iconY = y + hotbarBlockSize/2 - iconSize/2;
				ctx.drawImage(icon, iconX, iconY, iconSize, iconSize);

				ctx.save();
				ctx.fillStyle = "black";
				ctx.font = "10px Arial";
				ctx.textAlign = "center";
				ctx.fillText("x" + player.inventory.slots[i].count, iconX + iconSize, iconY);
				ctx.restore();
			}
		}
	}

	$(document).keydown(function(e) {
		var key = e.which;
		if(false) {}
		else if(key == "87")player.direction = "up";
		else if(key == "83")player.direction = "down";
		else if(key == "65")player.direction = "left";
		else if(key == "68")player.direction = "right";

		else if(key == "38")player.interactDirection = "up";
		else if(key == "40")player.interactDirection = "down";
		else if(key == "37")player.interactDirection = "left";
		else if(key == "39")player.interactDirection = "right";

		else if(key == "219" || key == "81")inventorySlot--;
		else if(key == "221" || key == "69")inventorySlot++;
		if(inventorySlot < 0)inventorySlot = 9;
		if(inventorySlot > 9)inventorySlot = 0;
	})

	$(document).mousedown(function(e) {
		var deltaX = e.clientX - player.x - w/2 - blockSize/2;
		var deltaY = e.clientY - player.y - h/2 - blockSize/2;
		if(Math.abs(deltaY) > Math.abs(deltaX))deltaX = 0;
		else deltaY = 0;

		var direction = "";
		if(false) {}
		else if(deltaX < 0)player.interactDirection = "left";
		else if(deltaX > 0)player.interactDirection = "right";
		else if(deltaY < 0)player.interactDirection = "up";
		else if(deltaY > 0)player.interactDirection = "down";
	})
})