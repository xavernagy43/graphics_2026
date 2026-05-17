**Graphics 2026 repo**

**Beadandó leírása**

A projekt maga egy egyszerű 3D játék megvalósítása OpenGL és SDL2 segítségével. A játék egy kietlen, elhagyatott területen játszódik, ahol a játékos feladata tekercsek összegyűjtése adott időn belül. A játék hangulatát sötét, kissé horrorisztikus jellegűre szeretném megoldani.

**Játékmenet**

A játékos egy karaktert irányít egy nyílt pályán, ahol összesen 8 darab tekercset kell megtalálnia és összegyűjtenie. A karakter zseblámpával tud világítani az F gomb lenyomásával.

Időlimit: 2 perc
Cél: az összes tekercs begyűjtése a megadott időn belül
A tekercsek a pályán elszórva helyezkednek majd el.

A játék akkor ér véget:

-ha a játékos összegyűjti az összes tekercset (győzelem),
-vagy ha lejár az idő (vereség).

**Segítség**

A súgó előhívható a H gomb lenyomásával.

**Pálya és környezet**

A játék egy elhagyatott területen játszódik, amely az alábbi elemeket tartalmazza:

-hold az égen
-rémisztő fák, fatörzsek
-csontváz
-egy elhagyatott ház
-egy sátor
-sátor mellett tábortűz (tűz animációval)
-tekercsek amik lebegnek

Ezek az objektumok statikus modellek, amelyeken a játékos nem tud áthaladni (ütközésvizsgálat fogja biztosítani).

A pályát köd egészíti ki, amely:
-fokozza a hangulatot
-opcionálisan ki és bekapcsolható a K gomb lenyomásával

A pálya fő fényforrása az égen található Hold lesz.

**HUD**

A képernyő egyik sarkában megjelenik egy HUD, amely folyamatosan frissül:
-hátralévő idő (pl. 01:45)
-összegyűjtött tekercsek száma (pl. 3/8)

**Hangok**

A játéknak lesz egy háttérzenéje, amit sdl_mixer segítségével tervezek megvalósítani.