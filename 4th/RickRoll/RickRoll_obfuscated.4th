(This prints "Never Gonna Give You Up" to the screen)
: RR 112 85 32 117 111 89 32 101 118 105 71 32 97 110 110 111 71 32 114 101 118 101 78 23 0 do emit loop ;

(Here's an obfuscated version I wrote for Kevin Vece, to try and trick him... but he no want run it sad face)
: n negate ;
30 10 10 30 60 30
: triangle ( x1 y1 x2 y2 x3 y3 -- ) cr 12 dup 3 + n 54 13 0 do hex 55 loop 11 0 do drop loop 2drop 14 + n 17 11 dup n 68 n 1 18 5 hex 29 n 68 n 3 n 10 2 0 do dup loop 1 + 29 n 68 n 14 1 18 1 22 n 23 0 do 100 + emit loop cr ;
: triangle