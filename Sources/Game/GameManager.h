#pragma once

#include "Framework/Debug.h"

#include <chrono>
#include <string>
#include <filesystem>

#include <nlohmann/json.hpp>

struct ScoreData final
{
    /* 플레이 날짜 */
    std::chrono::year_month_day date; 
    /* 플레이 시간 (초 단위) */
    float                       playTime;
    /* 사망 횟수 */
    int                         deathCount;
};

// 왜 문자열 변환 함수가 없냐;
static inline std::string ToString(const std::chrono::year_month_day& ymd)
{
    int year  = int(ymd.year());
    int month = unsigned(ymd.month());
    int day   = unsigned(ymd.day());

    char buf[11];
    std::snprintf(buf, sizeof(buf), "%04d-%02u-%02u", year, month, day);
    return std::string(buf);
}

// 왜 파싱 함수가 없냐;
static inline std::chrono::year_month_day ParseDate(const std::string& dateStr_)
{
    int year, month, day;
    sscanf_s(dateStr_.c_str(), "%d-%d-%d", &year, &month, &day);
    return std::chrono::year_month_day{std::chrono::year{year}, std::chrono::month{static_cast<unsigned int>(month)}, std::chrono::day{static_cast<unsigned int>(day)}};
}

class GameManager
{
public:
    static inline int currentLevel   = 0;
    static inline int maxLevel       = 7;

    static inline ScoreData curScoreData   = {};

    static constexpr size_t                 scoreCount = 5;
    static           inline std::vector<ScoreData> scores;

    static void Initialize()
    {
        currentLevel = 0;

        curScoreData.date       = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
        curScoreData.playTime   = 0.0f;
        curScoreData.deathCount = 0;
    }

    static void NextLevel()
    {
        currentLevel++;
    }

    static void LoadScoreData()
    {
        scores.clear();
        scores.reserve(scoreCount);

        std::ifstream file("Assets/Score/Scores.json");
        if (!file.is_open())
            return;

        nlohmann::json data;
        file >> data;
        file.close();

        if (data.empty())
        {
            Logger::Warn("No score data found.");
            return;
        }

        for (const auto& item : data)
        {
            ScoreData score;
            score.date       = ParseDate(item["date"]);
            score.playTime   = item["playTime"];
            score.deathCount = item["deathCount"];
            scores.push_back(score);
        }
    }

    static void SaveScoreData()
    {
        scores.push_back(curScoreData);
        std::sort(scores.begin(),
                  scores.end(),
                  [](const ScoreData& lhs_, const ScoreData& rhs_)
                  {
                      std::chrono::sys_days da = std::chrono::sys_days(lhs_.date);
                      std::chrono::sys_days db = std::chrono::sys_days(rhs_.date);
                      return da > db;
                  });

        if (scores.size() > scoreCount)
        {
            scores.resize(scoreCount);
        }

        nlohmann::json data = nlohmann::json::array();
        for (const auto& score : scores)
        {
            nlohmann::json item;
            item["date"]       = ToString(score.date);
            item["playTime"]   = score.playTime;
            item["deathCount"] = score.deathCount;
            data.push_back(item);
        }

        std::ofstream file("Assets/Score/Scores.json");
        if (!file.is_open())
        {
            return;
        }

        file << data.dump(4);
        file.close();
    }
};