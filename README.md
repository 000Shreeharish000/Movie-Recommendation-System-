# Movie Recommendation System

## Description
This project implements a personalized movie recommendation system in C++ using collaborative filtering. The system predicts the ratings a user would assign to unrated movies and recommends the top N movies.

## Features
- Load a user-movie ratings matrix from a CSV file.
- Calculate user-user similarity using Cosine Similarity.
- Predict ratings for unrated movies.
- Recommend top N movies for a user.

## Dataset
The `ratings.csv` file contains the user-movie ratings in the following format:
- Rows represent users.
- Columns represent movies.
- Values are ratings (1-5) or 0 for unrated movies.

## How to Run
1. Clone the repository or download the project files.
2. Place your `ratings.csv` file in the project directory.
3. Compile and run the program:
   ```bash
   g++ -o main main.cpp
   ./main
