package KittyDash;

public class Projectile extends Sprite {
	boolean shouldDie = false;
	int lifeSpan;
	int startX;

	public Projectile(String iconName, int _x, int _y, int _width, int _height, int _lifeSpan) {
		loadIcon(iconName);
		startX = _x;
		x = _x;
		y = _y;
		lifeSpan = _lifeSpan;
		icon = resizeImage(icon, _width, _height);
	}

	public void tick(double delta) {
		super.tick(delta);
		managePosition();
	}

	public void managePosition() {
		if(Math.abs(x-startX) > lifeSpan)
			shouldDie = true;
	}
}
