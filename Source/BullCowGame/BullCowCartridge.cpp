// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    //ClearScreen();

    if (bGameOver) {
        ClearScreen();
        SetupGame();
    }
    else {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{   
    // Welcome the player
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 3;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    //PrintLine(TEXT("The word is %s"), *HiddenWord);
    PrintLine(TEXT("You have %i Lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));
}

void UBullCowCartridge::EndGame(bool hasWon)
{
    ClearScreen();
    FString GameOver = hasWon ? TEXT(""): TEXT("Game over! ");
    PrintLine(hasWon ? TEXT("You have Won!") : TEXT("You have Lost!"));
    bGameOver = true;
    PrintLine(TEXT("%sThe hidden word was '%s'.\nPress enter to play again"), *GameOver, *HiddenWord);
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) {
        EndGame(true);
        return;
    }
    else {
        if (IsIsogram(Guess))
        {

        }
        PrintLine(TEXT("That word is wrong!\n"));
        if (--Lives > 0) {
            if (Guess.Len() != HiddenWord.Len())
                PrintLine(TEXT("The Hidden Word is %i characters long, try again!"), HiddenWord.Len());
            PrintLine(TEXT("You have %i Lives left!\n"), Lives);
            FBullCowCount Score = GetBullCows(Guess);
            PrintLine(TEXT("You have %i Bulls and %i Cows!\n"), Score.Bulls,Score.Cows);
        }
        else
        {
            EndGame(false);
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const 
{

    for (int32 i = 0; i < Word.Len() - 1; i++) {
        for (int32 j = i + 1; j < Word.Len(); j++) {
            if (Word[i] == Word[j])
                return false;
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{

    TArray<FString> ValidWords;
    for (FString Word : WordList) {
        if(Word.Len() == 5 && /*Word.Len() <= 8 &&*/ IsIsogram(Word))
            ValidWords.Emplace(Word);
           
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
    FBullCowCount Count;
    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i]) {
            Count.Bulls++;
            continue;
        }

        for (int j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j]) {
                Count.Cows++;
                break;
            }

        }
       
    }
    return Count;
}