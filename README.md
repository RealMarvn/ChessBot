# ♟️ ChessBot

**ChessBot** is a chess bot developed as part of my first programming project (PK1) at the University of Konstanz. This project dives into the complexities of chess move generation, including pseudolegal moves, castling, en passant, and iterative deepening for move depth evaluation.

## 🚀 Features

- **Pseudolegal Move Generation**: Generates moves without considering check, enhancing speed.
- **Castling and En Passant**: Supports special moves, providing a more complete set of chess rules.
- **Iterative Deepening**: Uses progressive depth evaluation to find the best move efficiently.

## 🏫 Project Background

This project was created as an introduction to programming concepts and problem-solving strategies. It combines classic chess rules with computational methods to give a practical application to theoretical programming knowledge from the University of Konstanz’s PK1 course.

## 🔍 How It Works

ChessBot uses the following strategies:

1. **Pseudolegal Moves**: Generates potential moves quickly by not checking for attacks on the king at first.
2. **Special Move Support**: Includes special moves like castling and en passant for a realistic move set including most of the classic chess rules.
3. **Iterative Deepening**: Allows the bot to make decisions under time constraints, prioritizing moves as it explores deeper levels.

## 🏗️ Project Structure

- Under ``/engine/`` you can find the whole bot with board logic and much more.
- Under ``/tests/`` you can find all tests I used to prove the MoveMaking is correct.

## 📝 Libaries

- Gtest from Google for automated tests