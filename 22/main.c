#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

enum SpellKind {
    MAGIC_MISSILE,
    DRAIN,
    SHIELD,
    POISON,
    RECHARGE,
    SPELL_COUNT,
};

struct Spell {
    int mana_cost;
    int damage;
    int heal;
    int shield;
    int mana_regen;
    int duration;
};

static const struct Spell spells[SPELL_COUNT] = {
    [MAGIC_MISSILE] = { 53, 4, 0, 0, 0,   1},
    [DRAIN]         = { 73, 2, 2, 0, 0,   1},
    [SHIELD]        = {113, 0, 0, 7, 0,   6},
    [POISON]        = {173, 3, 0, 0, 0,   6},
    [RECHARGE]      = {229, 0, 0, 0, 101, 5},
};

struct Wizard {
    int health;
    int mana;
    int timer[SPELL_COUNT];
};

struct Boss {
    int health;
    int damage;
};

void simulate_battle(bool part2, int *min_mana_cost, int current_cost, struct Wizard wizard, struct Boss boss, int turn) {
    if (current_cost > *min_mana_cost) {
        return;
    }

    int shield = 0;

    for (int i = 0; i < SPELL_COUNT; i++) {
        if (wizard.timer[i] == 0) continue;
        wizard.timer[i] -= 1;

        boss.health -= spells[i].damage;
        wizard.health += spells[i].heal;
        shield += spells[i].shield;
        wizard.mana += spells[i].mana_regen;
    }

    if (boss.health <= 0) {
        if (current_cost < *min_mana_cost) {
            *min_mana_cost = current_cost;
        }
        return;
    }

    if (turn % 2 == 1) {
        int boss_true_damage = boss.damage - shield;
        if (boss_true_damage <= 0) {
            boss_true_damage = 1;
        }

        wizard.health -= boss_true_damage;

        if (wizard.health <= 0) {
            return;
        }

        simulate_battle(part2, min_mana_cost, current_cost, wizard, boss, turn + 1);
        return;
    }

    if (part2) {
        wizard.health -= 1;
        if (wizard.health <= 0) {
            return;
        }
    }

    for (int i = 0; i < SPELL_COUNT; i++) {
        if (wizard.timer[i] != 0) continue;

        struct Wizard new_wizard = wizard;
        new_wizard.mana -= spells[i].mana_cost;
        new_wizard.timer[i] = spells[i].duration;

        if (new_wizard.mana < 0) continue;

        simulate_battle(part2, min_mana_cost, current_cost + spells[i].mana_cost, new_wizard, boss, turn + 1);
    }
}

int main(void) {
    struct Wizard wizard = {50, 500, {0}};
    struct Boss boss = {0};

    scanf("Hit Points: %d\n", &boss.health);
    scanf("Damage: %d\n", &boss.damage);

    int min_mana_cost = INT_MAX;
    simulate_battle(false, &min_mana_cost, 0, wizard, boss, 0);
    printf("Part 1: %d\n", min_mana_cost);

    min_mana_cost = INT_MAX;
    simulate_battle(true, &min_mana_cost, 0, wizard, boss, 0);
    printf("Part 2: %d\n", min_mana_cost);

    return 0;
}
