#pragma once

#include <iostream>
#include <thread>
#include <stop_token>
#include <future>
#include <queue>
#include <type_traits>
#include <functional>
#include <vector>
#include <memory>
#include <string>

namespace ff {
    template <typename R>
    class FFThreadPool {
    public:
        explicit FFThreadPool(unsigned int threadCount = std::thread::hardware_concurrency() - 1)
            : isRunning(false),
              isThreadAutoScale(false),
              busyThreadCount(0),
              isShowDebugMessage(false) {
            this->createThread(threadCount, true);
        }

        virtual ~FFThreadPool() {
            this->stop();
        }

    public: // Public
        template <typename Func, typename... Args>
        std::future<R> enqueue(Func&& func, Args&&... args) {
            auto task = std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
            std::future<R> result = task->get_future();
            {
                std::unique_lock<std::mutex> lockGuard(this->mutex);
                this->taskQueue.emplace([task]() {
                    (*task)();
                });
            }



            this->condition.notify_one();

            return result;
        }

        size_t getThreadCount() {
            return this->threadList.size();
        }

        void stop() {
            {
                std::unique_lock<std::mutex> lockGuard(this->mutex);
                this->isRunning = false;
            }
            this->condition.notify_all();
            this->threadAutoScaleWatchdog->request_stop();

            for (auto& thread : this->threadList) {
                if (thread.joinable() == false) {
                    continue;
                }
                thread.join();
            }
        }

        size_t getTaskQueueSize() {
            return this->taskQueue.size();
        }

        void setThreadAutoScale(bool isAutoScale) {
            this->isThreadAutoScale = isAutoScale; 

            if (this->isThreadAutoScale == true) {
                this->threadAutoScaleWatchdog = std::make_shared<std::jthread>([this](std::stop_token stopToken) {
                    while (stopToken.stop_requested() == false) {
                        if (this->isThreadAutoScale == true) {
                            if (this->busyThreadCount >= this->threadList.size()) {
                                this->debugMessage("ThreadPool busy create thread");
                                this->createThread(10, false);
                            }
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    }
                });
            } else {
                if (this->threadAutoScaleWatchdog != nullptr) {
                    this->threadAutoScaleWatchdog->request_stop();
                }
            }
        }

        void setShowDebugMessage(bool isDebugMessage) { this->isShowDebugMessage = isDebugMessage; }
        
    private: // Private
        void createThread(unsigned int threadCount, bool isDefaultThread) {
            this->debugMessage("createThread - count: " + std::to_string(threadCount) + " isDefaultThread: " + std::to_string(isDefaultThread));
            for (unsigned int i = 0; i < threadCount; i++) {

                // ====== 스레드 생성
                this->threadList.emplace_back([this, isDefaultThread]() {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lockGaurd(this->mutex);

                            if (this->isThreadAutoScale == true && isDefaultThread == false) {
                                 bool status = this->condition.wait_for(lockGaurd, std::chrono::seconds(10), [this]() {
                                    return this->isRunning == false || !this->taskQueue.empty();
                                 });
                                if (status == false) {
                                    // 기본 생성된 스레드가 아니면 10초동안 대기 후 타임아웃 시
                                    // 스케일링된 스레드가 자동 종료되게 함.
                                    this->debugMessage("Thread exit");
                                    return;
                                }
                            } else {
                                this->condition.wait(lockGaurd, [this]() {
                                    return this->isRunning == false || !this->taskQueue.empty();
                                });
                            }

                            if (this->isRunning == false &&
                                this->taskQueue.empty() == true) {
                                return;
                            }

                            task = std::move(this->taskQueue.front());
                            this->taskQueue.pop();
                        }

                        this->busyThreadCount++;
                        task();
                        this->busyThreadCount--;
                    }
                });
                // ============ 스레드 생성

                this->isRunning = true;
            }
        }


        void debugMessage(const std::string& message) {
            if (this->isShowDebugMessage) {
                std::cout << "Debug >> " << message << std::endl;
            }
        }


    private:
        bool isShowDebugMessage;

        std::atomic<bool> isThreadAutoScale;
        std::atomic<int> busyThreadCount;
        std::shared_ptr<std::jthread> threadAutoScaleWatchdog;

        bool isRunning;

        std::vector<std::thread> threadList;

        std::mutex mutex;
        std::condition_variable condition;

        std::queue<std::function<void()>> taskQueue;
    };
}