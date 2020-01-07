package KittyDash;

public class Player extends Lifeform {
	public Player() {
		loadIcon("Player_Icon.png");
		setAttackFrequency(0.1f);
	}

	public void tick(double delta) {
		super.tick(delta);
		if(wantsToAttack && canAttack())attack();
	}

	public void attack() {
		hasAttacked();
		Projectile projectile = new Projectile("Yarn.png", (int)x, (int)y, 30, 30, 8000);
		projectile.xAccel = 3.0f;
		projectile.xVel = 100;
		theGame.projectileCreated(projectile);
	}
}