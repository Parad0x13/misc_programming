game.PlayScreen = me.ScreenObject.extend({
	onResetEvent: function() {
		//me.levelDirector.loadLevel("area01");

		test = new me.Viewport(0, 0, 1, 1);
		test.fadeIn("#778899", 1);
		me.game.world.addChild(test);

		me.game.world.addChild(new game.BEG_Object("Ship", 1, 1));

		game.data.score = 0;

		// Add our HUD to the game world, add it last so that this is on top of the rest.
		// Can also be forced by specifying a "Infinity" z value to the addChild function.
		this.HUD = new game.HUD.Container();
		me.game.world.addChild(this.HUD);
		//this.HUD = new game.HUD.Inventory();
		//me.game.world.addChild(this.HUD);
	},

	onDestroyEvent: function() {
		me.game.world.removeChild(this.HUD);
	}
});