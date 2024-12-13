#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

// List of custom user names
std::vector<std::string> customUserNames = {"Manager", "TeamLead", "Developer", "Designer", "Analyst", "HR", "Intern", "CEO"};

// Load Ratings Matrix from CSV File
std::vector<std::vector<int>> loadRatingsMatrix(const std::string& filename, std::vector<std::string>& users, std::vector<std::string>& movies) {
    std::vector<std::vector<int>> ratings;
    std::ifstream file(filename);
    std::string line;

    // Reading first line (movie names)
    std::getline(file, line);
    std::stringstream ss(line);
    std::string movieName;
    std::getline(ss, movieName, ',');  // Skip the 'Position' label

    // Store movie names
    int movieIndex = 1;
    while (std::getline(ss, movieName, ',')) {
        movies.push_back("Movie" + std::to_string(movieIndex));  // Default name if you don't want real movie names
        movieIndex++;
    }

    // Reading the rest of the lines (user ratings)
    int userIndex = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<int> row;

        // Assign custom user names from the provided list
        if (userIndex < customUserNames.size()) {
            users.push_back(customUserNames[userIndex]); // Assign user name from custom list
        } else {
            users.push_back("User" + std::to_string(userIndex + 1)); // Default name if there are more users than the list
        }
        userIndex++;

        while (std::getline(ss, value, ',')) {
            row.push_back(std::stoi(value));
        }
        ratings.push_back(row);
    }

    return ratings;
}

// Calculate Cosine Similarity Between Two Users
double calculateSimilarity(const std::vector<int>& user1, const std::vector<int>& user2) {
    double dotProduct = 0, norm1 = 0, norm2 = 0;
    for (size_t i = 0; i < user1.size(); ++i) {
        if (user1[i] != 0 && user2[i] != 0) { // Only consider rated movies
            dotProduct += user1[i] * user2[i];
            norm1 += user1[i] * user1[i];
            norm2 += user2[i] * user2[i];
        }
    }
    if (norm1 == 0 || norm2 == 0) return 0; // Avoid division by zero
    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));
}

// Predict Rating for a User and Movie
double predictRating(const std::vector<std::vector<int>>& ratings, int user, int movie) {
    double numerator = 0, denominator = 0;
    for (size_t otherUser = 0; otherUser < ratings.size(); ++otherUser) {
        if (otherUser != user && ratings[otherUser][movie] != 0) {
            double similarity = calculateSimilarity(ratings[user], ratings[otherUser]);
            numerator += similarity * ratings[otherUser][movie];
            denominator += std::abs(similarity);
        }
    }
    return denominator == 0 ? 0 : numerator / denominator;
}

// Recommend Top N Movies for a User
std::vector<int> recommendMovies(const std::vector<std::vector<int>>& ratings, int user, int topN) {
    std::vector<std::pair<int, double>> predictions;
    for (size_t movie = 0; movie < ratings[user].size(); ++movie) {
        if (ratings[user][movie] == 0) { // Only predict for unrated movies
            double predictedRating = predictRating(ratings, user, movie);
            predictions.push_back({movie, predictedRating});
        }
    }

    // Sort movies by predicted rating in descending order
    std::sort(predictions.begin(), predictions.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Extract top N movie indices
    std::vector<int> recommendedMovies;
    for (size_t i = 0; i < std::min(predictions.size(), (size_t)topN); ++i) {
        recommendedMovies.push_back(predictions[i].first);
    }

    return recommendedMovies;
}

int main() {
    // Output the content type for HTML
    std::cout << "Content-Type: text/html\n\n";

    // Get the 'username' parameter from the CGI environment
    char* usernameParam = std::getenv("QUERY_STRING");
    int user = -1;  // Default user if invalid parameter
    if (usernameParam != nullptr) {
        user = std::atoi(usernameParam);  // Convert the user index from string to integer
    }

    // Load Ratings Matrix
    std::string filename = "ratings.csv";
    std::vector<std::string> users;  // Store user names
    std::vector<std::string> movies; // Store movie names
    std::vector<std::vector<int>> ratings = loadRatingsMatrix(filename, users, movies);

    // Recommend Top 3 Movies for the selected user
    int topN = 3;
    std::vector<int> recommendations = recommendMovies(ratings, user, topN);

    // Output the movie recommendations as HTML
    std::cout << "<h2>Recommendations for " << users[user] << ":</h2>";
    std::cout << "<ul>";
    for (int movie : recommendations) {
        std::cout << "<li>" << movies[movie] << "</li>";
    }
    std::cout << "</ul>";

    return 0;
}
