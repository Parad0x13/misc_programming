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

class Particle:
    def __init__(self, (x, y), size, mass=1):
        self.x = x
        self.y = y
        self.size = size
        self.mass = mass
        self.color = (0, 0, 255)
        self.thickness = 0
        self.speed = 0.01
        self.angle = math.pi/2
        self.drag = (self.mass/(self.mass + mass_of_air)) ** self.size
    def draw(self):
        pygame.draw.circle(screen, self.color, (int(self.x), int(self.y)), self.size, self.thickness)
    def move(self):
        self.x += math.sin(self.angle) * self.speed
        self.y -= math.cos(self.angle) * self.speed
        #(self.angle, self.speed) = addVectors((self.angle, self.speed), gravity)
        self.speed *= self.drag    
    def bounce(self):
        if self.x > width - self.size:
            self.x = 2*(width - self.size) - self.x
            self.angle = -self.angle
            self.speed *= elasticity
        elif self.x < self.size:
            self.x = 2*self.size - self.x
            self.angle = -self.angle
            self.speed *= elasticity
        if self.y > height - self.size:
            self.y = 2*(height - self.size) - self.y
            self.angle = math.pi - self.angle
            self.speed *= elasticity
        elif self.y < self.size:
            self.y = 2*self.size - self.y
            self.angle = math.pi-self.angle
            self.speed *= elasticity

def collide(p1, p2):
    dx = p1.x - p2.x
    dy = p1.y - p2.y
    distance = math.hypot(dx, dy)
    if distance < p1.size + p2.size:
        angle = math.atan2(dy, dx) + 0.5*math.pi
        total_mass = p1.mass + p2.mass
        (p1.angle, p1.speed) = addVectors((p1.angle, p1.speed*(p1.mass-p2.mass)/total_mass), (angle, 2*p2.speed*p2.mass/total_mass))
        (p2.angle, p2.speed) = addVectors((p2.angle, p2.speed*(p2.mass-p1.mass)/total_mass), (angle+math.pi, 2*p1.speed*p1.mass/total_mass))
        p1.speed *= elasticity
        p2.speed *= elasticity
        overlap = 0.5*(p1.size + p2.size - distance+1)
        p1.x += math.sin(angle)*overlap
        p1.y -= math.cos(angle)*overlap
        p2.x -= math.sin(angle)*overlap
        p2.y += math.cos(angle)*overlap

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

def addVectors((angle1, length1), (angle2, length2)):
    x = math.sin(angle1) * length1 + math.sin(angle2) * length2
    y = math.cos(angle1) * length1 + math.cos(angle2) * length2
    length = math.hypot(x, y)
    angle = 0.5 * math.pi - math.atan2(y, x)
    return (angle, length)

def findParticle(x, y):
    global particles
    for p in particles:
        if math.hypot(p.x-x, p.y-y) <= p.size:return p
    return None

determineParticles()

while running:
    handleEvents()
    render()
    logic()
    #Cheating with time.wait()!
    #pygame.time.wait(3)

pygame.quit()
