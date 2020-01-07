import pygame
import PyParticles

pygame.display.set_caption('Something')
(width, height) = (400, 400)
screen = pygame.display.set_mode((width, height))
env = PyParticles.Environment((width, height))

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: running = false
    pygame.display.flip()

pygame.exit()
