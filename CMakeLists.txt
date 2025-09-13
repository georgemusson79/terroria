cmake_minimum_required(VERSION 3.10)
project(terroria VERSION 0.1.0 LANGUAGES C CXX)

add_executable(terroria
    accessories.cpp arm.cpp ArmedEntity.cpp arrow.cpp tiles.cpp
    BossBar.cpp bows.cpp Bullet.cpp Camera.cpp Colliders.cpp
    cursor.cpp DEBUG.cpp Entity.cpp Entity1.cpp guiHeartandMana.cpp
    GUIInterface.cpp guns.cpp Initializer_functions.cpp Item.cpp
    itemPickup.cpp Items.cpp Mafia.cpp Main.cpp Main_Functions.cpp
    multiTileObject.cpp pickaxes.cpp Player.cpp swingingitem.cpp
    textureInfo.cpp TileEntities.cpp tilewall.cpp tilewalls.cpp
    Vector2.cpp Zombie.cpp "base tile.cpp"
)

# include dirs
target_include_directories(terroria PRIVATE
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/include"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_ttf-2.20.2/include"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_image-2.6.3/include"
)

# link libs
target_link_libraries(terroria PRIVATE
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/lib/x64/SDL2main.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/lib/x64/SDL2.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_ttf-2.20.2/lib/x64/SDL2_ttf.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_image-2.6.3/lib/x64/SDL2_image.lib"
)
