package KittyDash;

public class SprayBottle extends Lifeform {
	public SprayBottle() {
		loadIcon("SprayBottle.png");
		setAttackFrequency(1.5f);
	}

	public void tick(double delta) {
		super.tick(delta);
		if(theGame.player.y < y)yVel+=5;
		if(theGame.player.y > y)yVel-=5;

		if(Math.abs(theGame.player.y-y) < 50)
			wantsToAttack = true;
		if(wantsToAttack && canAttack())attack();
	}

	public void attack() {
		hasAttacked();
		Projectile projectile = new Projectile("SprayBottle.png", (int)x, (int)y, 30, 30, 8000);
		projectile.xAccel = 0.0f;
		projectile.xVel = -200;
		theGame.projectileCreated(projectile);
	}
}
