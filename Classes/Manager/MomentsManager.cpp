//
//  MomentsManager.cpp
//  pokerclient
//
//  Created by zhongyukeji on 16/4/14.
//
//

#include "MomentsManager.h"
#include "GameDataManager.h"
#include "file_utils.h"

MomentsManager* MomentsManager::__Moments_manager__ = nullptr;

MomentsManager::MomentsManager()
{
    register_events();
}

void MomentsManager::register_events()
{
    
}

void MomentsManager::add_moment(const msg::Moment& moment)
{
    msg::Moment cd;
    cd.CopyFrom(moment);
    
    uint32_t keyid = cd.momentid();
    
    auto iters = moment_records_.find(keyid);
    if(iters != moment_records_.end()) {
        iters->second.push_back(cd);
    }
    else {
        std::vector<msg::Moment> vec;
        vec.push_back(cd);
        moment_records_.insert(std::make_pair(keyid, vec));
    }
}

#pragma mark Save and Init
void MomentsManager::save_moment_record()
{
    if(moment_records_.empty()) {
        std::string user_cache = GDM->get_user_directory();
        file_utils::delete_directory(user_cache + "moment_data_record.a");
    }
    
    std::shared_ptr<msg::Processor_201_GetMoment_DOWN> pb_moment_datas(new msg::Processor_201_GetMoment_DOWN());
    int max_count = 100;
    
    for (auto iter = moment_records_.rbegin(); iter != moment_records_.rend(); ++iter) {
        std::list<msg::Moment> moments;
        for (auto& it : iter->second) {
            moments.push_back(it);
        }
        moments.sort([](const msg::Moment& c1, const msg::Moment& c2)->bool {
            return c1.time() < c2.time();
        });
        while (moments.size() > max_count) {
            moments.pop_front();
        }
        for (auto & it : moments){
            auto data = pb_moment_datas->add_moment();
            data->CopyFrom(it);
        }
    }
    
    
    if(moment_records_.size() > 0) {
        std::string result = "";
        
        bool succ = pb_moment_datas->SerializeToString(&result);
        if(succ) {
            std::string user_cache = GDM->get_user_directory();
            file_utils::write_file(user_cache + "moment_data_record.a", result);
        }
    }
    
}
void MomentsManager::init_moment_records()
{
    moment_records_.clear();
    std::string file_path = GDM->get_user_directory() + "moment_data_record.a";
    if(file_utils::file_exists(file_path)) {
        auto content = file_utils::read_file(file_path);
        msg::Processor_201_GetMoment_DOWN pb_Moment_datas;
        bool succ = pb_Moment_datas.ParseFromString(content);
        if(succ) {
            for (int j = 0; j < pb_Moment_datas.moment().size(); j++)
            {
                auto pb_cb = pb_Moment_datas.moment(j);
                msg::Moment cd;
                cd.CopyFrom(pb_cb);
                
                auto iter = moment_records_.find(cd.momentid());
                if(iter != moment_records_.end()) {
                    iter->second.push_back(cd);
                }
                else {
                    std::vector<msg::Moment> vec;
                    vec.push_back(cd);
                    moment_records_.insert(std::make_pair(cd.momentid(), vec));
                }
            }
        }
    }
}