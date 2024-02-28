#include "RecommendationSystem.h"
#include <algorithm>
#include <cmath>
#include <array>
#define INIT_BUCKET_SIZE 8



sp_movie RecommendationSystem::add_movie(const std::string& name,int year,
                                        const std::vector<double>& features)
{
  sp_movie movie = std::make_shared<Movie>(name, year);
  _movie_map[movie] = features;
  return movie;
}

sp_movie
RecommendationSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie = std::make_shared<Movie>(name, year);
  auto it = _movie_map.find (movie);
  if (it == _movie_map.end())
  {
    return nullptr;
  }
  return it->first;
}

std::set<sp_movie> RecommendationSystem::k_movies(const rank_map& rank, int k)
{
  std::set<sp_movie> best_wathed_movies;
  if(k >= int(rank.size()))
  {
    for(auto it = rank.begin(); it != rank.end(); it++)
    {
      best_wathed_movies.insert (it->first);
    }
    return best_wathed_movies;
  }
  //find the k best;
  rank_map sort_map = rank;
  for(int i=0; i<k;i++)
  {
    sp_movie max_movie;
    double max = 0.0;
    for(auto it = sort_map.begin(); it != sort_map.end(); it++)
    {
      if(it->second > max)
      {
        max = it->second;
        max_movie = it->first;
      }
    }
    best_wathed_movies.insert (max_movie);
    sort_map.erase (max_movie);
  }
  return best_wathed_movies;
}

double RecommendationSystem::predict_movie_score (const User &user, const
sp_movie &movie, int k)
{
  //part 1 - sum_rank of all viewed movied
  rank_map sum_ranked_movies(INIT_BUCKET_SIZE,sp_movie_hash,sp_movie_equal);
  double movie_dis = distance (_movie_map[movie]);
  rank_map temp = user.get_ranks();
  for(auto it = user.get_ranks().begin(); it != user.get_ranks().end(); it++)
  {
    //part 1.5 - sum the ranks and add to sum_ranked_movies
    int scalar = scalar_mult (_movie_map[movie], _movie_map[it->first]);
    double temp_dis = distance (_movie_map[it->first]);
    double new_rank = scalar / (movie_dis * temp_dis);
    sum_ranked_movies[it->first] = new_rank;
  }
  //part 2 - take only the k max
  std::set<sp_movie> best_movies_to_see = k_movies (sum_ranked_movies,k);
  //part 3 - sum the k max ranked
  double mone = 0;
  double mehane = 0;
  for(sp_movie movie : best_movies_to_see)
  {
    mone += sum_ranked_movies[movie] * temp[movie];
    mehane += sum_ranked_movies[movie];
  }
  double sum = mone / mehane;
  return sum;
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  //part 1 - crate best movies yet seen ranks
  rank_map best_movies(INIT_BUCKET_SIZE,sp_movie_hash,sp_movie_equal);
  for(auto it = _movie_map.begin(); it != _movie_map.end(); it++)
  {
    if(user.get_ranks().find(it->first) == user.get_ranks().end())
    {
      best_movies[it->first] = predict_movie_score(user,it->first,k);
    }
  }
  //part 1.5 -
  //part 2 - return max rank movie
  sp_movie max_rank_movie = best_movies.begin()->first;
  double max = best_movies.begin()->second;
  for(auto it = best_movies.begin(); it != best_movies.end();  it++)
  {
    if(it->second > max)
    {
      max_rank_movie = it->first;
      max = it->second;
    }
  }
  return max_rank_movie;
}

rank_map RecommendationSystem::get_avg_rank(const User& user)
{
  //create the avg_rank
  rank_map avg_rank(INIT_BUCKET_SIZE,sp_movie_hash,sp_movie_equal);
  double avg = 0.0;
  for(auto it = user.get_ranks().begin(); it != user.get_ranks().end(); it++)
  {
    avg += it->second;
  }
  avg /= int(user.get_ranks().size());
  for(auto it = user.get_ranks().begin(); it != user.get_ranks().end(); it++)
  {
      avg_rank[it->first] = (it->second) - avg;
  }
  return avg_rank;
}

std::vector<double> RecommendationSystem::create_avg_vector (const
rank_map&avg_rank)
{
  std::vector<double> final_rank;
  final_rank.reserve (_movie_map.begin()->second.size());
  for(int i=0; i<int(_movie_map.begin()->second.size()); i++)
  {
    final_rank.push_back (0.0);
  }
  for(auto it = avg_rank.begin(); it != avg_rank.end(); it++)
  {
    for(int j=0; j<int(_movie_map.begin()->second.size()); j++)
    {
      final_rank[j] += (it->second) * (_movie_map[it->first][j]);
    }
  }
  return final_rank;
}

int RecommendationSystem::scalar_mult(const std::vector<double>& lhs, const
std::vector<double>& rhs)
{
  int sum = 0;
  for(int i=0; i<int(lhs.size()); i++)
  {
    sum += (lhs[i] * rhs[i]);
  }
  return sum;
}

double RecommendationSystem::distance(std::vector<double>& vec)
{
  int sum = scalar_mult (vec,vec);
  return sqrt (sum);
}

std::map<sp_movie,double, func_comp> RecommendationSystem::new_movie_map
(std::vector<double>& final_rank, rank_map& avg_rank)
{
  std::map<sp_movie,double, func_comp> new_movie_to_see_map;
  double final_rank_dis = distance (final_rank);
  for(auto it = _movie_map.begin(); it != _movie_map.end(); it++)
  {
    if(avg_rank.find (it->first) == avg_rank.end())
    {
      int scalar = scalar_mult (final_rank, it->second);
      double temp_dis = distance (it->second);
      double new_rank = scalar / (final_rank_dis * temp_dis);
      new_movie_to_see_map[it->first] = new_rank;
    }
  }
  return new_movie_to_see_map;
};

sp_movie RecommendationSystem::recommend_by_content (const User &user)
{
  //part 1 - create avg_rank for user
  rank_map avg_rank = get_avg_rank (user);
  //part 2 - crate the vector
  std::vector<double> final_rank = create_avg_vector (avg_rank);
  //part 3
  std::map<sp_movie,double, func_comp> new_movie_to_see_map = new_movie_map
      (final_rank, avg_rank);
  //part 4 - find max
  sp_movie max_rank_movie = new_movie_to_see_map.begin()->first;
  double max = new_movie_to_see_map.begin()->second;
  for(auto it = new_movie_to_see_map.begin();
                                      it != new_movie_to_see_map.end();  it++)
  {
    if(it->second > max)
    {
      max_rank_movie = it->first;
      max = it->second;
    }
  }
  return max_rank_movie;
}

std::ostream& operator<<(std::ostream& os,const RecommendationSystem& rhs)
{
  for(auto it = rhs._movie_map.begin(); it != rhs._movie_map.end(); it++)
  {
    os << (*it->first);
  }
  return os;
}
