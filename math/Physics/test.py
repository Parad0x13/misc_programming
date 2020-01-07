import pygame, random, math

background_color = (255, 255, 255)
(width, height) = (400, 600)
gravity = (math.pi, 0.002)
mass_of_air = 0.2
elasticity = 0.75

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('Tutorial 1')

particleCount = 20
particles = []
selected_particle = None

running = True

def determineParticles():
    global particles
    particles = []
    for n in range(particleCount):
        positionFound = False
        while positionFound == False:
            size = random.randint(10, 20)
            x = random.randint(size, width-size)
            y = random.randint(size, height-size)
            obscurred = False
            for particle in particles:
                deltaX = x-particle.x
                deltaY = y-particle.y
                actualDistance = math.hypot(deltaX, deltaY)
                requiredDistance = size+particle.size
                if actualDistance < requiredDistance:obscurred = True
            if obscurred == False:
                density = random.randint(1, 20)
                particle = Particle((x, y), size, density*size**2)
                particle.speed = random.random()
                particle.angle = random.uniform(0, math.pi*2)
                particle.color = (200-density*10, 200-density*10, 255)
                particles.append(particle)
                positionFound = True

def render():
    screen.fill(background_color)
    for particle in particles:particle.draw()
    pygame.display.flip()

def handleEvents():
    global running
    global selected_particle
    for event in pygame.event.get():
        if event.type == pygame.QUIT:running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            (mouseX, mouseY) = pygame.mouse.get_pos()
            selected_particle = findParticle(mouseX, mouseY)
        elif event.type == pygame.MOUSEBUTTONUP:
            selected_particle = None
    if selected_particle:
        (mouseX, mouseY) = pygame.mouse.get_pos()
        dx = mouseX - selected_particle.x
        dy = mouseY - selected_particle.y
        selected_particle.angle = math.atan2(dy, dx) + 0.5*math.pi
        selected_particle.speed = math.hypot(dx, dy) * 0.1

def logic():
    for i, particle in enumerate(particles):
        particle.move()
        particle.bounce()
        for particle2 in particles[i+1:]:
            collide(particle, particle2)

determineParticles()

while running:
    handleEvents()
    render()
    logic()
    #Cheating with time.wait()!
    #pygame.time.wait(3)

pygame.quit()
