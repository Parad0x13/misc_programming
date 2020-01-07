package KittyDash;

public class Lifeform extends Sprite {
	Game theGame;
	boolean wantsToAttack = false, needsToDie = false;
	private long attackSpeed;

	long lastAttack = 0;

	public Lifeform() {
		setAttackFrequency(0.5f);
	}

	public void setAttackFrequency(float frequency) {
		attackSpeed = (long)(frequency * 1000000000);
	}

	public void tick(double delta) {
		super.tick(delta);
		manageCollisionWithWalls();
	}

	public boolean canAttack() {
		long now = System.nanoTime();
		if(now - lastAttack > attackSpeed)return true;
		return false;
	}

	public void hasAttacked() {
		long now = System.nanoTime();
		lastAttack = now;
		wantsToAttack = false;
	}

	public void attack() {
		//
	}

	public void manageCollisionWithWalls() {
		float impactSlowDownModifier = 0.75f;
		if(y < 0) {
			y = 0;
			yVel = -yVel * impactSlowDownModifier;
		}
		if(y > theGame.frame.getHeight()-icon.getHeight()/2.0f) {
			y = theGame.frame.getHeight()-icon.getHeight()/2.0f;
			yVel = -yVel * impactSlowDownModifier;
		}
	}
}
