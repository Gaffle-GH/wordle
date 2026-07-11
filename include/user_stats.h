#ifndef USER_STATS_H
#define USER_STATS_H

extern int totalGames;
extern int wins;
extern int totalAttempts;
extern int wins_per_attempt[6];

void updateStats(bool won, int attempts);
void stats(int totalGames, int wins, int attempts);

#endif
