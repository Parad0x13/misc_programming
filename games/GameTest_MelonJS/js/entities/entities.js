Inventory = me.Object.extend({
	init: function() {
		this.contents = [];
	},

	add: function(toAdd) {
		this.contents.push(toAdd);
	},

	log: function() {
		console.log("Inventory consists of:");
		console.log(this.contents);
	}
});

game.BEG_Object = me.GUI_Object.extend({
	init: function(BEG_Tag, xLoc, yLoc, settings) {
		this._super(me.GUI_Object, 'init', [xLoc * BLOCK_SIZE, yLoc * BLOCK_SIZE, {image: me.loader.getImage("BEG_Default")}]);
		this.anchorPoint = {x: 0, y: 0};

		if(BEG_Tag == "Ship") {
			this.image = me.loader.getImage("BEG_Ship");
			this.inventory = new Inventory();
			this.inventory.add("Stuff");
		}
	},

	onClick: function(e) {
		console.log("Clicked on object");
		this.inventory.log();
		return false;
	}
});