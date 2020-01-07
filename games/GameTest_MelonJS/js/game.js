BLOCK_SIZE = 16;

var game = {
	data : {
		score : 0
	},

	"onload" : function () {
		// Initialize the video.
		//if (!me.video.init(200, 200, {wrapper : "screen", scale : "auto"})) {
		if (!me.video.init(100, 100, {scale: "auto"})) {
			alert("Your browser does not support HTML5 canvas.");
			return;
		}

		// add "#debug" to the URL to enable the debug Panel
		if (me.game.HASH.debug === true) {
			window.onReady(function () {
				me.plugin.register.defer(this, me.debug.Panel, "debug", me.input.KEY.V);
			});
		}

		me.audio.init("mp3,ogg");

		// Set a callback to run when loading is complete.
		me.loader.onload = this.loaded.bind(this);

		me.loader.preload(game.resources);

		// Initialize melonJS and display a loading screen.
		me.state.change(me.state.LOADING);
	},

	"loaded" : function () {
		me.state.set(me.state.MENU, new game.TitleScreen());
		me.state.set(me.state.PLAY, new game.PlayScreen());

		// Old code here since I removed PlayerEntity. But what does registering with the pool do?
		//me.pool.register("mainPlayer", game.PlayerEntity);

		me.state.change(me.state.PLAY);
	}
};