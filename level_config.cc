#include "level_config.h"

LevelConfig GetLevelConfig(int level)
{

    LevelConfig config;
    
    config.level_number = level;

    // Cada nivel aumenta enemigos
    config.amount_enemies = 3 + level;

    // Cada 4 niveles cambia tipo enemigo
    int stage = (level - 1) / 2;

    switch (stage)
    {
    case 0:
        config.enemy_type = ENE::KMeteorites;
        config.nave_sprite_set = 0;
        break;
    case 1:
        config.enemy_type = ENE::KFurballs;
        config.nave_sprite_set = 0;
        break;
    case 2:
        config.enemy_type = ENE::KDarts;
        config.nave_sprite_set = 1;
        break;
    case 3:
        config.enemy_type = ENE::KBubbles;
        config.nave_sprite_set = 1;
        break;
    case 4:
        config.enemy_type = ENE::KUfo;
        config.nave_sprite_set = 2;
        break;
    case 5:
        config.enemy_type = ENE::KFlower;
        config.nave_sprite_set = 2;
        break;
    case 6:
        config.enemy_type = ENE::KAlien;
        config.nave_sprite_set = 2;
        break;

    default:
        config.enemy_type = ENE::KJets;
        config.nave_sprite_set = 2;
        break;
    }

    return config;
}