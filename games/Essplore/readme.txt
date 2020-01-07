File Saving
Turns out data serialization is a difficult thing to properly do... so I'm going to cheat
Not going to worry about scalability
Not going to worry about filesize
Not going to worry about backwards compatibility

I'm not going to try and make the game very scalable, it's just for fun so with that in mind, here's my plan for gamesaveing

World (~=water, P=player, .=dirt):

~~~~~~~~~~~~~~~~~~~~
~~~~~~......~~~~~~~~
~~~~..........~~~~~~
~~..............~~~~
~~~~......P....~~~~~
~~~~~~~.....~~~~~~~~
~~~~~~~~~~~~~~~~~~~~

Entire world will be in one chunk, outside the world will be unpassable objects

Data will be serialized by starting with the topleft tile and go down to the bottomright tile

#define TAG_STARTOFITEMS	777
#define TAG_NEWITEM			778
#define TAG_ENDOFITEMS		779
#define TAG_ITEMDATASTART	780
#define TAG_ITEMDATAEND		781

Data will be stored in no particular order to the items location, instead all items will look like this:
As a list of integers separated by a space
After each item will be the option of adding an arbitrary amount of data to be interpreted by the item itself
For example an item might want to specify if it has dropped or not

TAG_NEWITEM TAG xLocation yLocation [TAG_ITEMDATASTART] [int] ... [int] [TAG_ITEMDATAEND]

After world data will be player inventory data right away

#define TAG_INVENTORY_START	780
#define TAG_INVENTORY_END	790

Data will then proceed with item, then count nothing else.