#pragma once

// IF POSSIBLE, DO NOT USE THE EMPTY VALUE
// THIS IS ONLY USED FOR DEFAULT CASES
UENUM(BlueprintType)
enum EBox : int { empty, grass = 'G', rock = 'R', tree = 'T', mushroom = 'M' };