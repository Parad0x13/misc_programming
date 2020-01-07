package KittyDash;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;

@SuppressWarnings("serial")
public class Game extends JPanel {
	protected static final int VK_LEFT = 0;
	final int TARGET_FPS = 60;
	final long nanoSecondsInSecond = 1000000000;
	final long OPTIMAL_TIME = nanoSecondsInSecond / TARGET_FPS;

	BufferedImage carpet1, carpet2;
	AffineTransform carpet1at = new AffineTransform();
	AffineTransform carpet2at = new AffineTransform();
	Player player = new Player();
	boolean travelingUp = false, travelingDown = false;
	float playerBoostSpeed = 10.0f, playerDragMultiplier = 0.1f;
	long lastLoopTime = System.nanoTime();
	long lastFpsTime = 0;
	int fps = 0;
	static JFrame frame;
	ArrayList<Projectile>projectiles;
	ArrayList<Lifeform>enemies;
	int carpetSpeed = 8;

	private void gameLogic(double delta) {
		player.tick(delta);

		manageProjectiles(delta);
		manageEnemies(delta);
		manageCarpet();
	}

	private void manageProjectiles(double delta) {
		ArrayList<Projectile>projectilesToRemove = new ArrayList<Projectile>();
		for(Projectile projectile : projectiles) {
			projectile.tick(delta);
			if(projectile.shouldDie)projectilesToRemove.add(projectile);
		}
		for(Projectile projectile : projectilesToRemove)projectiles.remove(projectile);
	}

	private void manageEnemies(double delta) {
		ArrayList<Lifeform>enemiesToRemove = new ArrayList<Lifeform>();
		for(Lifeform enemy : enemies) {
			enemy.tick(delta);
			if(enemy.needsToDie)enemiesToRemove.add(enemy);
		}
		for(Lifeform enemy : enemiesToRemove)enemies.remove(enemy);
	}

	private void handleInput() {
		if(travelingUp) {
			player.affectVelocity(0.0f, playerBoostSpeed);
			player.isAcceleratingUp = true;
		}
		if(travelingDown) {
			player.affectVelocity(0.0f, -playerBoostSpeed);
			player.isAcceleratingDown = true;
		}
		if(!travelingUp && !travelingDown) {
			player.giveDrag(playerDragMultiplier);
		}
	}

	private void gameLoop() throws InterruptedException {
		long now = System.nanoTime();
		long updateLength = now - lastLoopTime;
		lastLoopTime = now;
		double delta = updateLength / ((double)OPTIMAL_TIME);
		lastFpsTime += updateLength;
		fps++;

		if(lastFpsTime >= nanoSecondsInSecond) {
			lastFpsTime = 0;
			fps = 0;
		}

		gameLogic(delta);
		handleInput();
		render();

		long pause = (lastLoopTime-System.nanoTime() + OPTIMAL_TIME)/1000000;
		if(pause > 0) {
			try {
				Thread.sleep(pause);
			} catch(InterruptedException ex) {
				System.out.println("Something bad happened");
			}
		}
	}

	public void paint(Graphics g) {
		super.paint(g);
		Graphics2D g2d = (Graphics2D)g;
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

		g2d.drawImage(carpet1, carpet1at, null);
		g2d.drawImage(carpet2, carpet2at, null);

		player.render(g2d);

		for(Lifeform enemy : enemies)
		    enemy.render(g2d);
		for(Projectile projectile : projectiles)
		    projectile.render(g2d);
	}

	public void manageCarpet() {
		carpet1at.translate(-carpetSpeed, 0);
		carpet2at.translate(-carpetSpeed, 0);
		if(carpet1at.getTranslateX() < -carpet1.getWidth())carpet1at.translate(carpet1.getWidth()*2, 0);
		if(carpet2at.getTranslateX() < -carpet2.getWidth())carpet2at.translate(carpet2.getWidth()*2, 0);
	}

	public void render() {
		repaint();
	}

	public void projectileCreated(Projectile projectile) {
		if(projectile != null) {
			projectiles.add(projectile);
		}
	}

	public Game() {
		KeyListener listener = new KeyListener() {
			public void keyTyped(KeyEvent e) {
				//
			}

			public void keyPressed(KeyEvent e) {
				if(e.getKeyCode() == KeyEvent.VK_D) {
					player.wantsToAttack = true;
					//player.attack();
				}
				if(e.getKeyCode() == KeyEvent.VK_W)travelingUp = true;
				if(e.getKeyCode() == KeyEvent.VK_S)travelingDown = true;
			}

			public void keyReleased(KeyEvent e) {
				if(e.getKeyCode() == KeyEvent.VK_W) {
					travelingUp = false;
					player.isAcceleratingUp = false;
				}
				if(e.getKeyCode() == KeyEvent.VK_S) {
					travelingDown = false;
					player.isAcceleratingDown = false;
				}
			}
		};
		addKeyListener(listener);
		setFocusable(true);

		projectiles = new ArrayList<Projectile>();
		enemies = new ArrayList<Lifeform>();

		try {
			URL iconImgUrl = this.getClass().getResource("/resources/Carpet.jpg");
			carpet1 = ImageIO.read(iconImgUrl);
			carpet2 = ImageIO.read(iconImgUrl);
		} catch(IOException ex) {}
		carpet1at.translate(0, 0);
		carpet2at.translate(carpet2.getWidth(), 0);
	}

	public static void main(String[] args) throws InterruptedException {
		final int width = 800, height = 600;
		frame = new JFrame("They Kitty Dash Game!");
		Game game = new Game();
		frame.add(game);
		frame.setSize(width, height);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		boolean gameActive = true;
		game.player.x = 75;
		game.player.y = (int)(frame.getHeight()/2);
		game.player.theGame = game;

		SprayBottle sprayBottle = new SprayBottle();
		sprayBottle.x = 600;
		sprayBottle.y = game.player.y;
		sprayBottle.theGame = game;
		game.enemies.add(sprayBottle);

		while(gameActive) {
			game.gameLoop();
		}
	}
}