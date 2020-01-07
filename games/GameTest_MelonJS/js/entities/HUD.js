game.HUD = game.HUD || {};

game.HUD.Container = me.Container.extend({
	init: function() {
		this._super(me.Container, 'init');

		this.isPersistent = true;

		// make sure we use screen coordinates
		this.floating = true;

		this.name = "HUD";

		this.addChild(new game.HUD.ScoreItem(15, 15));
	}
});

game.HUD.ScoreItem = me.Renderable.extend({
	init: function(x, y) {
		// (size does not matter here)
		this._super(me.Renderable, 'init', [x, y, 10, 10]);

		this.font = new me.BitmapFont("32x32_font", 32);
		this.font.set("right");

		// local copy of the global score
		this.score = -1;
	},

	update : function () {
		// we don't do anything fancy here, so just
		// return true if the score has been updated
		if (this.score !== game.data.score) {
			this.score = game.data.score;
			return true;
		}
		return false;
	},

	draw : function (context) {
		// draw it baby !
		this.font.draw(context, game.data.score, this.pos.x, this.pos.y);
	}
});

game.HUD.Inventory = me.Container.extend({
	init: function() {
		this._super(me.Container, 'init');

		this.isPersistent = true;

		// make sure we use screen coordinates
		this.floating = true;

		this.name = "HUD";
	}
});