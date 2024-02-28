//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H
#include "User.h"
#include <map>
#include <set>


struct func_comp
{
    bool operator()(const sp_movie& a, const sp_movie& b) const
    {
      return (*a < *b);
    }
};

typedef std::map<sp_movie, std::vector<double>, func_comp> movie_featuers;

class RecommendationSystem
{
 private:
  movie_featuers _movie_map;

  rank_map get_avg_rank(const User& user);
  std::vector<double> create_avg_vector( const rank_map& avg_rank);
  int scalar_mult(const std::vector<double >& lhs, const
  std::vector<double>& rhs);
  double distance(std::vector<double>& vec);
  std::map<sp_movie,double, func_comp> new_movie_map(std::vector<double>&
      final_rank, rank_map& avg_rank);
  std::set<sp_movie> k_movies(const rank_map& rank, int k);
public:
	//explicit RecommendationSystem()
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year ,const
    std::vector<double>& features);


    /**
     * a function that calculates the movie with highest score based on
     * movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest predicted score
     * based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using item  cf
     * procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;

    friend std::ostream& operator<<(std::ostream& os, const
    RecommendationSystem& rhs);
};


#endif //RECOMMENDATIONSYSTEM_H
