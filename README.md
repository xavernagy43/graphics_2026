**Graphics 2026 repo**

**Beadandó leírása**

A projekt maga egy egyszerű 3D játék megvalósítása OpenGL és SDL2 segítségével. A játék egy kietlen, elhagyatott területen játszódik, ahol a játékos feladata könyvek összegyűjtése adott időn belül. A játék hangulatát sötét, kissé horrorisztikus jellegűre szeretném megoldani.

**Játékmenet**

A játékos egy karaktert irányít egy nyílt pályán, ahol összesen 8 darab könyvet kell megtalálnia és összegyűjtenie.

Időlimit: 2 perc
Cél: az összes könyv begyűjtése a megadott időn belül
A könyvek a pályán elszórva helyezkednek majd el.

A játék akkor ér véget:

-ha a játékos összegyűjti az összes könyvet (győzelem),
-vagy ha lejár az idő (vereség).

**Pálya és környezet**

A játék egy elhagyatott területen játszódik, amely az alábbi elemeket fogja tartalmazni a tervek szerint:

-kiszáradt, rémisztő fák
-egy kút
-csontvázak
-egy elhagyatott ház

Ezek az objektumok statikus modellek, amelyeken a játékos nem tud áthaladni (ütközésvizsgálat fogja biztosítani).

A pályát köd egészíti ki, amely:
-fokozza a hangulatot
-opcionálisan ki és bekapcsolható a beállításokban

A pálya fő fényforrása az égen található Hold lesz.

**HUD**

A képernyő egyik sarkában megjelenik egy HUD, amely folyamatosan frissül:
-hátralévő idő (pl. 01:45)
-összegyűjtött könyvek száma (pl. 3/8)

**Menü rendszer**

A játék indulásakor egy főmenü fog megjelenni az alábbi opciókkal:

Start – játék indítása
Help – súgó megjelenítése
Settings – beállítások
Quit – kilépés

**Hangok**

A játéknak lesz egy háttérzenéje, amit sdl_mixer segítségével tervezek megvalósítani.

A technológia részek fejlesztés közben kerülnek még majd hozzáadásra.