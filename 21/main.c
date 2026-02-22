#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

struct Stats {
    int health;
    int damage;
    int armor;
};

struct Item {
    int cost;
    int damage;
    int armor;
};

static const struct Item weapons[] = {
    {8, 4, 0},
    {10, 5, 0},
    {25, 6, 0},
    {40, 7, 0},
    {74, 8, 0},
};

static const struct Item armor[] = {
    {13, 0, 1},
    {31, 0, 2},
    {53, 0, 3},
    {75, 0, 4},
    {102, 0, 5},
};

static const struct Item rings[] = {
    {25, 1, 0},
    {50, 2, 0},
    {100, 3, 0},
    {20, 0, 1},
    {40, 0, 2},
    {80, 0, 3},
};

bool does_player_win(struct Stats player, struct Stats boss) {
    int player_true_damage = player.damage - boss.armor;
    if (player_true_damage <= 0) {
        player_true_damage = 1;
    }

    int boss_true_damage = boss.damage - player.armor;
    if (boss_true_damage <= 0) {
        boss_true_damage = 1;
    }

    int player_win_turns = rint(ceilf((float)boss.health / (float)player_true_damage));
    int boss_win_turns = rint(ceilf((float)player.health / (float)boss_true_damage));

    return player_win_turns <= boss_win_turns;
}

int minmax(struct Stats player, struct Stats boss, bool has_weapon, bool has_armor, int ring1, int ring2) {
    if (does_player_win(player, boss)) {
        return 0;
    }

    int min_cost = INT_MAX;

    if (!has_weapon) {
        for (size_t i = 0; i < ARRAY_LENGTH(weapons); i++) {
            struct Stats new_player = player;
            new_player.damage += weapons[i].damage;

            int cost = minmax(new_player, boss, true, has_armor, ring1, ring2);
            if (cost == INT_MAX) continue;

            cost += weapons[i].cost;

            if (cost < min_cost) {
                min_cost = cost;
            }
        }

        return min_cost;
    }

    if (!has_armor) {
        for (size_t i = 0; i < ARRAY_LENGTH(armor); i++) {
            struct Stats new_player = player;
            new_player.armor += armor[i].armor;

            int cost = minmax(new_player, boss, has_weapon, true, ring1, ring2);
            if (cost == INT_MAX) continue;

            cost += armor[i].cost;

            if (cost < min_cost) {
                min_cost = cost;
            }
        }
    }

    if (ring1 < 0) {
        for (size_t i = 0; i < ARRAY_LENGTH(rings); i++) {
            struct Stats new_player = player;
            new_player.damage += rings[i].damage;
            new_player.armor += rings[i].armor;

            int cost = minmax(new_player, boss, has_weapon, has_armor, i, ring2);
            if (cost == INT_MAX) continue;

            cost += rings[i].cost;

            if (cost < min_cost) {
                min_cost = cost;
            }
        }
    } else if (ring2 < 0) {
        for (size_t i = ring1 + 1; i < ARRAY_LENGTH(rings); i++) {
            struct Stats new_player = player;
            new_player.damage += rings[i].damage;
            new_player.armor += rings[i].armor;

            int cost = minmax(new_player, boss, has_weapon, has_armor, ring1, i);
            if (cost == INT_MAX) continue;

            cost += rings[i].cost;

            if (cost < min_cost) {
                min_cost = cost;
            }
        }
    }

    return min_cost;
}

int maxmin(struct Stats player, struct Stats boss, bool has_weapon, bool has_armor, int ring1, int ring2) {
    if (does_player_win(player, boss)) {
        return INT_MIN;
    }

    int max_cost = 0;

    if (!has_weapon) {
        for (size_t i = 0; i < ARRAY_LENGTH(weapons); i++) {
            struct Stats new_player = player;
            new_player.damage += weapons[i].damage;

            int cost = maxmin(new_player, boss, true, has_armor, ring1, ring2);
            if (cost == INT_MIN) continue;

            cost += weapons[i].cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
        }

        return max_cost;
    }

    if (!has_armor) {
        for (size_t i = 0; i < ARRAY_LENGTH(armor); i++) {
            struct Stats new_player = player;
            new_player.armor += armor[i].armor;

            int cost = maxmin(new_player, boss, has_weapon, true, ring1, ring2);
            if (cost == INT_MIN) continue;

            cost += armor[i].cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
        }
    }

    if (ring1 < 0) {
        for (size_t i = 0; i < ARRAY_LENGTH(rings); i++) {
            struct Stats new_player = player;
            new_player.damage += rings[i].damage;
            new_player.armor += rings[i].armor;

            int cost = maxmin(new_player, boss, has_weapon, has_armor, i, ring2);
            if (cost == INT_MIN) continue;

            cost += rings[i].cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
        }
    } else if (ring2 < 0) {
        for (size_t i = ring1 + 1; i < ARRAY_LENGTH(rings); i++) {
            struct Stats new_player = player;
            new_player.damage += rings[i].damage;
            new_player.armor += rings[i].armor;

            int cost = maxmin(new_player, boss, has_weapon, has_armor, ring1, i);
            if (cost == INT_MIN) continue;

            cost += rings[i].cost;

            if (cost > max_cost) {
                max_cost = cost;
            }
        }
    }

    return max_cost;
}

int main(void) {
    struct Stats player = {100, 0, 0};
    struct Stats boss = {0};

    scanf("Hit Points: %d\n", &boss.health);
    scanf("Damage: %d\n", &boss.damage);
    scanf("Armor: %d\n", &boss.armor);

    printf("Part 1: %d\n", minmax(player, boss, false, false, -1, -1));
    printf("Part 2: %d\n", maxmin(player, boss, false, false, -1, -1));

    return 0;
}
