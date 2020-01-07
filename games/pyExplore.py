#First game coded with python, nothing but a learning experience!
#1.0
import random
import readline

#Constants
COMMON =    1
UNCOMMON =  2
RARE =      3

DEFAULT_ITEM =  'DEFAULT'
WEAPON_ITEM =   'WEAPON'
RING_ITEM =     'RING'

class Item:
    name = 'defaultItem'
    itemType = DEFAULT_ITEM
    def __init__(self):
        self.stats = {'mana':0, 'strength':0}
    def printInfo(self):
        print self.name
        if self.stats['mana'] > 0: print "\tMana(+%i)" % (self.stats['mana'])
        if self.stats['strength'] > 0: print "\tStrength(+%i)" % (self.stats['strength'])
    def giveRarity(self, rarity):
        if(rarity == COMMON): self.name+='(C)'
        elif(rarity == UNCOMMON): self.name+='(U)'
        elif(rarity == RARE): self.name+='(R)'

class Ring(Item):
    name = 'Ring'
    itemType = RING_ITEM
    def __init__(self, rarity=None):
        Item.__init__(self)
        if rarity == None: rarity = random.randint(1, 3)
        if rarity == COMMON: self.stats['mana'] = random.randrange(1, 5)
        elif rarity == UNCOMMON: self.stats['mana'] = random.randrange(3, 7)
        elif rarity == RARE: self.stats['mana'] = random.randrange(5, 9)
        self.giveRarity(rarity)

class Sword(Item):
    name = 'Sword'
    itemType = WEAPON_ITEM
    def __init__(self, rarity=None):
        Item.__init__(self)
        if rarity == None: rarity = random.randint(1, 3)
        if rarity == COMMON: self.stats['strength'] = random.randrange(3, 7)
        elif rarity == UNCOMMON: self.stats['strength'] = random.randrange(5, 9)
        elif rarity == RARE: self.stats['strength'] = random.randrange(7, 11)
        self.giveRarity(rarity)

class Inventory:
    items = []
    def __init__(self):
        pass
    def addItem(self, item):
        self.items.append(item)
    def addItems(self, *items):
        self.items.extend(*items)
    def printInfo(self):
        for item in self.items:
            item.printInfo()
    def sort(self): self.items.sort(key=lambda item: item.name)

class Equipment:
    items = []
    def __init__(self):
        pass
    def printInfo(self):
        for item in self.items:
            print "Item type: %s" % item.itemType
            item.printInfo()
    def equipItem(self, theItem):
        canEquip = True
        dropped = None
        for item in self.items:
            if item.itemType == theItem.itemType:
                dropped = item
                #canEquip = false
        if canEquip:
            self.items.append(theItem)
        else:
            print 'Cannot equip'
        if dropped:
            print 'Dropping an item!'
        return dropped

class Player:
    health = 100;
    stats = {'mana':0, 'strength':0}
    inventory = Inventory()
    equipment = Equipment()
    def __init__(self):
        pass
        #self.inventory.addItems((Ring(), Sword()))
    def viewInventory(self):
        print 'Player\'s Inventory:'
        self.inventory.printInfo()
    def viewEquipment(self):
        print 'Player\'s Equipment:'
        self.equipment.printInfo()
    def giveItem(self, item):
        self.inventory.addItem(item)
    def equipItem(self, item):
        dropped = self.equipment.equipItem(item)
        if dropped:
            self.inventory.addItem(dropped)
    def getStrength(self):
        value = self.stats['strength']
        for item in self.equipment.items:
            value += item.stats['strength']
        return value
    def getMana(self):
        value = self.stats['mana']
        for item in self.equipment.items:
            value += item.stats['mana']
        return value
    def viewStats(self):
        print 'Player Stats:'
        print 'Health: ' + str(self.health)
        print 'Strength: ' + str(self.getStrength())
        print 'Mana: ' + str(self.getMana())

COMMANDS = ['help', 'quit', 'exit', 'inventory', 'equipment', 'stats']
COMMANDS.sort()
def complete(text, state):
    for cmd in COMMANDS:
        if cmd.startswith(text):
            if not state:
                return cmd
            else:
                state = -1
readline.parse_and_bind("tab: complete")
readline.set_completer(complete)

class Game:
    iterations = 0
    gameActive = True
    command = None
    player = Player()
    player.equipItem(Ring())
    player.equipItem(Sword())
    def MainLoop(self):
        while self.gameActive:
            self.GetInput()
            self.ProcessInput()
    def GetInput(self):
        self.command = raw_input('Command: ')
        self.command = self.command.lower()
    def ProcessInput(self):
        if self.command == 'help':
            print 'Permitted Commands are:'
            print COMMANDS
        elif self.command == 'quit' or self.command == 'exit':
            print 'Goodbye!'
            self.gameActive = False
        elif self.command == 'inventory':
            self.player.viewInventory()
        elif self.command == 'equipment':
            self.player.viewEquipment()
        elif self.command == 'stats':
            self.player.viewStats()
        else:
            print 'That command is not permitted, try \'help\''

game = Game()
game.MainLoop()
