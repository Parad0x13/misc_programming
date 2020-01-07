package KittyDash;

import java.awt.AlphaComposite;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.net.URL;
import javax.imageio.ImageIO;
import java.io.IOException;

public class Sprite {
	boolean isAcceleratingUp = false, isAcceleratingDown = false;
	BufferedImage thrustUp, thrustDown;
	int defaultWidth, defaultHeight;
	float x, y, centerX, centerY;
	float xVel, yVel;
	float xAccel, yAccel;
	BufferedImage icon;

	public static BufferedImage resizeImage(final BufferedImage image, int width, int height) {
		// Ensure that the resizedImage will correctly scale to input image
		int newWidth = width, newHeight = height;
		float ratio = 1.0f;
		if(image.getWidth() > image.getHeight()) {
			ratio = (float)image.getHeight()/(float)image.getWidth();
			newHeight *= ratio;
		}
		if(image.getWidth() < image.getHeight()) {
			ratio = (float)image.getWidth()/(float)image.getHeight();
			newWidth *= ratio;
		}
        final BufferedImage bufferedImage = new BufferedImage(newWidth, newHeight, BufferedImage.TYPE_INT_ARGB);
        final Graphics2D graphics2D = bufferedImage.createGraphics();
        graphics2D.setComposite(AlphaComposite.Src);
        graphics2D.drawImage(image, 0, 0, newWidth, newHeight, null);
        graphics2D.dispose();

        return bufferedImage;
    }

	public void affectVelocity(float deltaX, float deltaY) {
		xVel += deltaX;
		yVel += deltaY;
	}

	public void giveDrag(float drag) {
		float finalDrag = 1.0f - drag;
		xVel *= finalDrag;
		yVel *= finalDrag;
	}

	public void loadIcon(String name) {
		icon = loadImage(name);
		icon = resizeImage(icon, defaultWidth, defaultHeight);
		thrustUp = loadImage("Thrust_Up.png");
		thrustUp = resizeImage(thrustUp, (int)(icon.getWidth()*0.5f), (int)(icon.getWidth()*0.5f));
	}

	public BufferedImage loadImage(String name) {
		BufferedImage retVal = null;
		try {
			URL iconImgUrl = this.getClass().getResource("/resources/" + name);
			retVal = ImageIO.read(iconImgUrl);
		} catch(IOException ex) {
			//
		}
		return retVal;
	}

	public Sprite() {
		x = 0;
		y = 0;
		xVel = 0.0f;
		yVel = 0.0f;
		defaultWidth = 100;
		defaultHeight = 100;
		loadIcon("Default.png");
	}

	public void render(Graphics2D g2d) {
		if(isAcceleratingUp) {
			AffineTransform at = new AffineTransform();
			at.translate((int)(x-thrustUp.getWidth()/2), (int)(y+icon.getHeight()/2));
			g2d.drawImage(thrustUp, at, null);
		}
		if(isAcceleratingDown) {
			AffineTransform at = new AffineTransform();
			at.translate((int)(x-thrustUp.getWidth()/2), (int)(y-icon.getHeight()/2));
			at.rotate(Math.toRadians(180));
			at.translate(-thrustUp.getWidth(), 0);
			g2d.drawImage(thrustUp, at, null);
		}
		g2d.drawImage(icon, (int)(x-icon.getWidth()/2.0f), (int)(y-icon.getHeight()/2.0f), null);
	}

	public void tick(double delta) {
		double time = 1 - delta;
		xVel += xVel * xAccel * time;
		yVel += yVel * yAccel * time;
		x += xVel*time;
		y -= yVel*time;
	}
}