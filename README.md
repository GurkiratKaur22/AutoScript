# AutoScript
> A smart spell checker written in C using KMP String Matching and Edit Distance (Dynamic Programming).

---

## What It Does
- Types a word → checks if it exists in dictionary using **KMP**
- If misspelled → suggests closest correct word using **Edit Distance (DP)**

```
Enter word: algorythm
[KMP]  "algorythm" NOT found in dictionary.
[DP]   Running Edit Distance to find closest word...
Did you mean: "algorithm"? (edit distance: 1)

Enter word: hello
[KMP]  "hello" found in dictionary.
Result: CORRECT SPELLING [OK]
```

---

## Algorithms Used

### 1. KMP String Matching — Unit 4 (String Matching Algorithms)
- Builds a **Failure Array (LPS)** from the input word
- Searches dictionary efficiently without restarting on mismatch
- Used for: **exact word lookup**

### 2. Edit Distance — Unit 3 (Dynamic Programming)
- Builds a **2D DP table** counting minimum insert/delete/replace operations
- Compares input against every dictionary word
- Suggests word with **lowest edit distance (≤ 3)**
- Used for: **closest word suggestion**

---

## Project Structure
```
AutoScript/
├── main.c           ← all code (KMP + Edit Distance + main loop)
└── dictionary.txt   ← word list
```

---

## How To Run

**Step 1: Clone the repo**
```bash
git clone https://github.com/GurkiratKaur22/AutoScript.git
cd AutoScript
```

**Step 2: Compile**
```bash
gcc main.c -o AutoScript
```

**Step 3: Run**
```bash
./AutoScript
```

> **Windows users:** Install [MinGW](https://mingw-w64.org) for GCC compiler.

---

## Add More Words
Just open `dictionary.txt` and add any word on a new line. No code changes needed.

---

## Course Info
- **Subject:** Analysis of Algorithms (AoA)
- **Algorithms:** KMP (Unit 4) + Dynamic Programming — Edit Distance (Unit 3)
- **Language:** C only
