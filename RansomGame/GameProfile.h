#pragma once
struct game_profile
{
	const char* game_title;
	int difficulty_memory_address;
	int score_memory_address;
	int minimum_score;
};

enum difficulty { easy, normal, difficult, lunatic };