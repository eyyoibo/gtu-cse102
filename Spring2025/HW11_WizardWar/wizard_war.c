#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_CLASS_LEN 30
#define MAX_SPELLS 50
#define NUM_WIZARD_SPELLS 3

typedef struct {
    char name[MAX_NAME_LEN];
    char magic_class[MAX_CLASS_LEN];
    int min_damage;
    int max_damage;
    int min_mana;
    int max_mana;
} Spell;

typedef struct {
    char name[MAX_NAME_LEN];
    char magic_class[MAX_CLASS_LEN];
    Spell spells[NUM_WIZARD_SPELLS];
    int hp;
    int mana;
    int alive;
    int stats[3]; /* [0]=damage dealt, [1]=mana spent, [2]=recoveries */
} Wizard;

int calculate_score(int stats[], int n);
void duel(Wizard *attacker, Wizard *defender);
int load_spellbook(const char *filename, Spell spellbook[], int max_spells);
void init_wizard(Wizard *w, const char *name, const char *magic_class, const Spell spellbook[], int total_spells);

int calculate_score(int stats[], int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return stats[0] * 2 + calculate_score(stats, 0);
    }
    if (n == 2) {
        return stats[1] * 1 + calculate_score(stats, 1);
    }
    if (n == 3) {
        return (-stats[2] * 3) + calculate_score(stats, 2);
    }
    return 0;
}

void duel(Wizard *attacker, Wizard *defender) {
    int spell_idx;
    int mana_cost;
    int damage;
    Spell *spell;

    if (!attacker->alive || !defender->alive) {
        return;
    }

    spell_idx = rand() % NUM_WIZARD_SPELLS;
    spell = &attacker->spells[spell_idx];

    mana_cost = spell->min_mana + (rand() % (spell->max_mana - spell->min_mana + 1));
    damage = spell->min_damage + (rand() % (spell->max_damage - spell->min_damage + 1));

    if (attacker->mana < mana_cost) {
        int recovery = 10 + (rand() % 11); /* 10 - 20 */
        attacker->mana += recovery;
        attacker->stats[2]++;
        printf("-------------------------------------------------\n");
        printf("%s is low on mana and meditates...\n", attacker->name);
        printf("%s recovers %d mana. Current mana: %d\n", attacker->name, recovery, attacker->mana);
    } else {
        int bonus = 0;
        if (strcmp(attacker->magic_class, spell->magic_class) == 0) {
            bonus = 5;
            damage += bonus;
        }

        attacker->mana -= mana_cost;
        attacker->stats[0] += damage;
        attacker->stats[1] += mana_cost;

        defender->hp -= damage;
        if (defender->hp <= 0) {
            defender->hp = 0;
            defender->alive = 0;
        }

        printf("-------------------------------------------------\n");
        printf("%s casts %s on %s!\n", attacker->name, spell->name, defender->name);
        if (bonus > 0) {
            printf("Elemental Mastery Bonus (+5 damage)!\n");
        }
        printf("Damage: %d | %s's HP: %d | %s's Mana: %d\n",
               damage, defender->name, defender->hp, attacker->name, attacker->mana);
    }

    if (defender->hp <= 0) {
        printf("-------------------------------------------------\n");
        printf("Winner: %s the %s Wizard!\n\n", attacker->name, attacker->magic_class);
        return;
    }

    duel(defender, attacker);
}

int load_spellbook(const char *filename, Spell spellbook[], int max_spells) {
    FILE *fp;
    char line[256];
    int count = 0;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open %s\n", filename);
        return 0;
    }

    while (count < max_spells && fgets(line, sizeof(line), fp)) {
        char *token;
        char name[MAX_NAME_LEN];
        char magic_class[MAX_CLASS_LEN];
        int min_d, max_d, min_m, max_m;

        /* Remove newline */
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;

        token = strtok(line, ",");
        if (!token) continue;
        strncpy(name, token, MAX_NAME_LEN - 1);
        name[MAX_NAME_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(magic_class, token, MAX_CLASS_LEN - 1);
        magic_class[MAX_CLASS_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        min_d = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        max_d = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        min_m = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        max_m = atoi(token);

        strncpy(spellbook[count].name, name, MAX_NAME_LEN);
        strncpy(spellbook[count].magic_class, magic_class, MAX_CLASS_LEN);
        spellbook[count].min_damage = min_d;
        spellbook[count].max_damage = max_d;
        spellbook[count].min_mana = min_m;
        spellbook[count].max_mana = max_m;
        count++;
    }

    fclose(fp);
    return count;
}

void init_wizard(Wizard *w, const char *name, const char *magic_class, const Spell spellbook[], int total_spells) {
    int i, j;
    int chosen[NUM_WIZARD_SPELLS];

    strncpy(w->name, name, MAX_NAME_LEN - 1);
    w->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(w->magic_class, magic_class, MAX_CLASS_LEN - 1);
    w->magic_class[MAX_CLASS_LEN - 1] = '\0';

    w->hp = 100;
    w->mana = 100;
    w->alive = 1;
    w->stats[0] = 0;
    w->stats[1] = 0;
    w->stats[2] = 0;

    for (i = 0; i < NUM_WIZARD_SPELLS; i++) {
        int idx;
        int duplicate;
        do {
            duplicate = 0;
            idx = rand() % total_spells;
            for (j = 0; j < i; j++) {
                if (chosen[j] == idx) {
                    duplicate = 1;
                    break;
                }
            }
        } while (duplicate);
        chosen[i] = idx;
        w->spells[i] = spellbook[idx];
    }
}

int main(void) {
    Spell spellbook[MAX_SPELLS];
    int total_spells;
    Wizard w1;
    Wizard w2;
    int score1;
    int score2;

    srand((unsigned int)time(NULL));

    total_spells = load_spellbook("spellbook.txt", spellbook, MAX_SPELLS);
    if (total_spells < NUM_WIZARD_SPELLS) {
        printf("Error: Not enough spells in spellbook.txt (found %d)\n", total_spells);
        return 1;
    }

    init_wizard(&w1, "Merlin", "fire", spellbook, total_spells);
    init_wizard(&w2, "Frostina", "ice", spellbook, total_spells);

    printf("Wizard Duel Begins: %s vs %s!\n", w1.name, w2.name);
    duel(&w1, &w2);

    score1 = calculate_score(w1.stats, 3);
    score2 = calculate_score(w2.stats, 3);

    printf("Battle Summary:\n");
    printf("%s — Damage: %d | Mana Spent: %d | Recoveries: %d | Score: %d\n",
           w1.name, w1.stats[0], w1.stats[1], w1.stats[2], score1);
    printf("%s — Damage: %d | Mana Spent: %d | Recoveries: %d | Score: %d\n",
           w2.name, w2.stats[0], w2.stats[1], w2.stats[2], score2);

    return 0;
}
