**Graphics 2026 repo**

**Beadandó leírása**

A projekt maga egy egyszerű 3D játék megvalósítása OpenGL és SDL2 segítségével. A játék egy kietlen, elhagyatott területen játszódik, ahol a játékos feladata tekercsek összegyűjtése adott időn belül. A játék hangulata sötét, kissé horrorisztikus jellegű.

**Futtatáshoz szükséges teendők**

A következő Google Drive linken található mappát le kell tölteni:
https://drive.google.com/drive/folders/1wlpeQqoe2lp8MvhbuGfo3o4iLknpOn2j?usp=drive_link

A letöltött fájlok közül az **assets.rar** állományt ki kell csomagolni a beadandó projekt főkönyvtárába. Az assets mappa tartalmazza a program futásához szükséges erőforrásokat:

- 3D modellek
- textúrák
- betűtípus (.ttf)

Ezek nélkül a program nem fog megfelelően működni.
Emellett megtalálható az **sdl2_ttf.rar** fájl is, amely a szövegmegjelenítéshez szükséges SDL2_ttf könyvtár állományait tartalmazza.

A telepítéshez a következő lépések szükségesek:

1. Az sdl2_ttf/include/SDL2 mappából másold át az SDL_ttf.h fájlt a következő helyre:
        MinGW/include/SDL2

2. Az sdl2_ttf/lib mappából másold át az .a és .la fájlokat ide:
        MinGW/lib

3. Az sdl2_ttf/bin mappából másold át a .dll fájlt a beadandó projekt főkönyvtárába.

A fenti lépések elvégzése után a projekt lefordítható a c_sdk_220203 shell elindítását követően. A beadandó projekt mappájában add ki a következő parancsot:

- make

A fordítás befejezése után a játék a következő paranccsal indítható:

- game.exe

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
-egy elhagyatott ház
-egy sátor
-sátor mellett tábortűz (tűz animációval)
-tekercsek amik lebegnek

Ezek az objektumok statikus modellek, amelyek nagy részén a játékos nem tud áthaladni, ütközésvizsgálat biztosítja.

A pályát köd egészíti ki, amely:
-fokozza a hangulatot
-opcionálisan ki és bekapcsolható a K gomb lenyomásával

A pálya fő fényforrása az égen található Hold lesz.

**HUD**

A képernyő egyik sarkában megjelenik egy HUD, amely folyamatosan frissül:
-hátralévő idő (pl. 01:45)
-összegyűjtött tekercsek száma (pl. 3/8)