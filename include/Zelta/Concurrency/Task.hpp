/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Task.hpp
 * Author: rafa
 *
 * Created on January 14, 2018, 3:01 AM
 */

#ifndef ZELTALIB_TASK_HPP
#define ZELTALIB_TASK_HPP

namespace zt {

    class Task {
    public:
        Task();
        //Task(const Task& orig);
        virtual ~Task();

        virtual bool work() = 0;
    private:

    };

}

#endif /* TASK_HPP */

