#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>


class Movie{
public:
    typedef enum {
        MovieType_Film,
        MovieType_Tv
    }MovieType;

    Movie(std::string name, int year,MovieType type){
        this->name = name;
        this->year = year;
        this->type = type;
    }
    std::string getName() const{
        return name;
    }
    int getYear() const{
        return year;
    }
    void setYear(int year){
        this->year = year;
    }
    MovieType getMovieType() const{
        return type;
    }
private:
    std::string name;
    MovieType type;
    int year;
};



// Hard/frustrating to "always" : always these iterators (begin,end)
// I mean how often so we sort all after the fifth one std::sort(v.begin() + 5, v.end())
// no chaining
// hello C++20 = ranges and views
// Lazy-evaluation (deferred) = LINQ in C#/streams in Java
// var numbers = new List<int> { 1, 2, 3, 4, 5 };
// var result = numbers.Where(n => n > 2).Select(n => n * 2);
// numbers.Add(6);
// The logic runs NOW, and it will include the '6' we added!
//        foreach (var item in query)
// {
//            Console.WriteLine($"Found: {item}");
// }

// std::ranges::sort(std::views::drop(v, 5));

int main(){



    std::vector<Movie> greatMovies{
        Movie("The Mummy returns",2001,Movie::MovieType::MovieType_Film),
        Movie("Fast and Furious 7",2014,Movie::MovieType::MovieType_Film),
        Movie("Fast & Furious Presents: Hobbs & Shaw",2019,Movie::MovieType::MovieType_Film),
        Movie("Young Rock",2021,Movie::MovieType::MovieType_Tv),
        Movie("The Scorpion King", 2002, Movie::MovieType::MovieType_Film),
        Movie("Welcome to the Jungle", 2003, Movie::MovieType::MovieType_Film),
        Movie("Walking Tall", 2004, Movie::MovieType::MovieType_Film),
        Movie("Be Cool", 2005, Movie::MovieType::MovieType_Film),
        Movie("Doom", 2005, Movie::MovieType::MovieType_Film),
        Movie("Gridiron Gang", 2006, Movie::MovieType::MovieType_Film),
        Movie("The Game Plan", 2007, Movie::MovieType::MovieType_Film),
        Movie("Get Smart", 2008, Movie::MovieType::MovieType_Film),
        Movie("Race to Witch Mountain", 2009, Movie::MovieType::MovieType_Film),
        Movie("Tooth Fairy", 2010, Movie::MovieType::MovieType_Film),
        Movie("Fast Five", 2011, Movie::MovieType::MovieType_Film),
        Movie("Journey 2: The Mysterious Island", 2012, Movie::MovieType::MovieType_Film),
        Movie("G.I. Joe: Retaliation", 2013, Movie::MovieType::MovieType_Film),
        Movie("Hercules", 2014, Movie::MovieType::MovieType_Film),
        Movie("San Andreas", 2015, Movie::MovieType::MovieType_Film),
        Movie("Central Intelligence", 2016, Movie::MovieType::MovieType_Film),
        Movie("Moana", 2016, Movie::MovieType::MovieType_Film), // Röst
        Movie("Jumanji: Welcome to the Jungle", 2017, Movie::MovieType::MovieType_Film),
        Movie("Rampage", 2018, Movie::MovieType::MovieType_Film),
        // TV-serie
        Movie("Ballers", 2015, Movie::MovieType::MovieType_Tv), // Startår, en TV-serie        
        Movie("Star Trek: Voyager",2000,Movie::MovieType::MovieType_Tv),
    };

    // 1. Filtera ut alla filmer (inga TV-serier)
    // 2. Sortera filmerna på år
    // 3. ta dom 5 senaste filmerna
    // 4. Lägg in en vector<string> med namnen på dom 5 senaste filmerna  <- START PULLING

    // Many people code like thisd
    std::vector<Movie> myLatestMovies;
    for (auto &movie : greatMovies)
    {
        if(movie.getMovieType() == Movie::MovieType::MovieType_Film){
            myLatestMovies.push_back(movie);
        }
    }
    std::sort(myLatestMovies.begin(), myLatestMovies.end(), [](const Movie& a, const Movie& b){
        return a.getYear() > b.getYear();
    }); // SORT IN PLACE 

    std::vector<Movie> justFivemyLatestMovies;
    for(int i = 0; i < 5 && i < myLatestMovies.size(); ++i){
        justFivemyLatestMovies.push_back(myLatestMovies[i]);
    }
    // justFivemyLatestMovies will contain the 5 latest movies, but we have done a lot of work to get there, and we have created a lot of temporary objects along the way, which can be inefficient and hard to read
 
    std::vector<std::string> mylatestMovieNames;
    for(const auto& movie : justFivemyLatestMovies){
        mylatestMovieNames.push_back(movie.getName());
    }
    for(const auto& name : mylatestMovieNames){
        std::cout << name << "\n";
    }

    // while(myLatestMovies.size() > 5){
    //     myLatestMovies.pop_back();
    // }

    


    // BETTER ?
    std::vector<Movie> latestMovies;
    std::copy_if(greatMovies.begin(), greatMovies.end(), std::back_inserter(latestMovies), [](const Movie& movie){
        return movie.getMovieType() == Movie::MovieType::MovieType_Film;
    });
    std::sort(latestMovies.begin(), latestMovies.end(), [](const Movie& a, const Movie& b){
        return a.getYear() > b.getYear();
    });
    if(latestMovies.size() > 5){
        latestMovies.erase(latestMovies.begin() + 5, latestMovies.end()); // Keep only the 5 latest movies
    }
    std::vector<std::string> latestMovieNames;
    std::transform(latestMovies.begin(), latestMovies.end(), std::back_inserter(latestMovieNames), [](const Movie& movie){
        return movie.getName();
    });
    for(const auto& name : latestMovieNames){
        std::cout << name << "\n";
    }


    // rewrite using views and ranges
    std::cout << "\nUsing C++20 ranges and views:\n";
    std::vector<Movie> sortedMovies(greatMovies.begin(), greatMovies.end());
    std::ranges::sort(sortedMovies, [](const Movie& a, const Movie& b){
        return a.getYear() > b.getYear();
    });
    
    auto latestMoviesView = sortedMovies 
        | std::views::filter([](const Movie& movie){ return movie.getMovieType() == Movie::MovieType::MovieType_Film; })
        | std::views::take(5)
        | std::views::transform([](const Movie& movie){ return movie.getName(); });
    for(const auto& name : latestMoviesView){
        std::cout << name << "\n";
    }
    // Lazy evaluation = more efficient and more readable, but it can be harder to debug and understand for some people, especially if they are not used to functional programming concepts. It also requires C++20 support, which may not be available in all compilers or projects.




    return 0;
}