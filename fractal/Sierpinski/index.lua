-- Made by Bryan... A.K.A. Bryanandhallie
-- A serpinski's triangle inside another's center, different colors
-- Variables
canvas = Image.createEmpty(480, 272)
X = 240
Y = 272/2
XX = 240
YY = 272/2
XXX = 0
YYY = 0
XXXX = 479
YYYY = 0
rand = 0
num = 100
iter = 0
red = Color.new(255, 0, 0)
green = Color.new(0, 255, 0)
blue = Color.new(0, 0, 255)
yellow = Color.new(255, 255, 0)
lightblue = Color.new(0, 255, 255)
pink = Color.new(255, 0, 155)
draw=1

-- Functions
function wipe()
  screen:clear()
  screen:flip()
  screen:clear()
  screen:flip()
end
function drawpoint()
  rand = math.random(3)
  if rand == 1 then X = X-((X-240)/2) Y = Y-((Y-0)/2) end
  if rand == 2 then X = X-((X-0)/2) Y = Y-((Y-271)/2) end
  if rand == 3 then X = X-((X-479)/2) Y = Y-((Y-271)/2) end
  canvas:pixel(X, Y, red)
  if rand == 1 then XX = XX-((XX-122)/2) YY = YY-((YY-136)/2) end
  if rand == 2 then XX = XX-((XX-357)/2) YY = YY-((YY-136)/2) end
  if rand == 3 then XX = XX-((XX-239)/2) YY = YY-((YY-269)/2) end
  canvas:pixel(XX, YY, yellow)
  if rand == 1 then XXX = XXX-((XXX-0)/2) YYY = YYY-((YYY-0)/2) end
  if rand == 2 then XXX = XXX-((XXX-240)/2) YYY = YYY-((YYY-0)/2) end
  if rand == 3 then XXX = XXX-((XXX-0)/2) YYY = YYY-((YYY-271)/2) end
  canvas:pixel(XXX, YYY, blue)
  if rand == 1 then XXXX = XXXX-((XXXX-240)/2) YYYY = YYYY-((YYYY-0)/2) end
  if rand == 2 then XXXX = XXXX-((XXXX-479)/2) YYYY = YYYY-((YYYY-0)/2) end
  if rand == 3 then XXXX = XXXX-((XXXX-479)/2) YYYY = YYYY-((YYYY-271)/2) end
  canvas:pixel(XXXX, YYYY, green)
end

-- Before Main Loop
wipe()

-- Main Program
while true do
  iter = iter+num
  pad = Controls.read()
  if pad:l() then canvas:pixel(math.random(479), math.random(271), lightblue) end
  if pad:r() then canvas:pixel(math.random(479), math.random(271), pink) end
  if pad:up() then num = num+10 end
  if pad:down() and num>0 then num = num-10 end
  if pad:start() then break end
  if pad:circle() then canvas:clear() iter=0 end
  if pad:triangle() then draw=2 end

  if draw==1 then
    screen:clear()
    screen:blit(0, 0, canvas)
    screen:flip()
    screen.waitVblankStart()
  end
  if draw==2 then
    screen:clear()
    screen:print(100, 100, "Iterations: " ..iter, blue)
    screen:print(100, 150, "NumVal: " ..num, blue)
    screen:flip()
    screen.waitVblankStart()
  end

  draw=1
  if num>0 then for d=0, num do drawpoint() end end
end