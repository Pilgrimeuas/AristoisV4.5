#pragma once
#include "Module.h"
#include <ctime>

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;

	std::string deathMsg[15] = {
		">Come on buddy, don't be complacent, it's no big deal to kill me",
		">Shut up, your mouth stinks worse than your smelly socks",
		">Don't think you'll be able to kill me next time, the smug clown",
		">Don't take yourself seriously, it's really not",
		">Think about how you won before you taunt, stupid pig",
		">Do you really think you're powerful, God doesn't think so",
		">You fool who can only run away, it was just luck that you killed me",
		">Before you start to feel proud of killing me, count on your hands how many totems you've popped by me",
		">Do you really think you're strong, unless you swear to God first that you're a fool",
		">you're nothing without your clown client",
		">People who say  'EZ'  will only prove that he are an autistic person",
		">People with autism often repeat a sentence because they have extreme low self-esteem",
		">Your pity has manifested itself when you are busy taunting me without you knowing it",
		">Maybe you'll laugh at me, but that's it",
		">Kid, have you finished your homework?",
	};

	std::string killMsg[15] = {
		">I think you're ridiculous, you're going to fight me so seriously",
		">Is there really someone like you who likes to prove that he is strong but worthless?",
		">Oh my God, I can beat you and pigs can fly into the sky",
		">All I can say is that you can't beat me, just as the Germans can never win every war",
		">I think p0mhub is more like your warmest home than anarchy servers",
		">Bro, take a break when you are tired of playing, don't tire yourself out",
		">Maybe you should think about whether you need a better client to compete with me, such as onix",
		">When I fight you, I'm curious not about how to defeat you, but about how many totems you have",
		">If you are a newfag to continue the game, if you are an oldfag to quit the game, delete minecraft",
		">Maybe you should have been killed by me before the battle began, and there is no value in fighting you",
		">If you're sad to be killed by me, think more about how you killed me instead of me killing you, which is very effective for your happiness",
		">If you think my skills are all up to my client, you're completely wrong"
		">You actually killed me just now, and you were actually daydreaming just now",
		">You lost, it's a pity, I heard that input/hub can get 32k kit + 1000 extra damage",
		">Oh my God, you brought 100 totems, is that true?",
		">Now I will tell everyone a truth lag = I am a weak chicken. crash = I'm gay",
	};

public:
	int kc = 0;
	int dc = 0;
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = true;

	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};