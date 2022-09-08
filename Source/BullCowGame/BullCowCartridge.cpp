// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();

    PrintLine(TEXT("The number of possible words is: %i."), GetValidWords(Words).Num());
} 

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull Cow!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() -1)];
    PrintLine(TEXT("The hidden word is: %s."), *HiddenWord);
    Lives = HiddenWord.Len();
    bGameOver = false;

    // PrintLine(TEXT("The hidden word is %s"), *HiddenWord);
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Lives left: %i"), Lives);
    PrintLine(TEXT("Type your guess and \npress enter to continue"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You won"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Hidden word is %i letter word! Try again"), HiddenWord.Len());
        PrintLine(TEXT("Lives left: %i"), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters! Try again"));
        PrintLine(TEXT("Lives left: %i"), Lives);
        return;
    }

    --Lives;
    if (Lives <= 0)
    {
        PrintLine(TEXT("No lifes left!"));
        PrintLine(TEXT("Hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Lives left: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++; 
                break;
            }
        }
    }
    return Count;
}