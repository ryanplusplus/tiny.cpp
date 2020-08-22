/*!
 * @file
 * @brief
 */

#include "tiny/TimerGroup.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "double/TimeSourceDouble.h"

using namespace tiny;

static TimerTicks restart_ticks;

TEST_GROUP(TimerGroup)
{
  TimeSourceDouble time_source;

  Timer timer_1;
  Timer timer_2;
  Timer timer_3;
  Timer timer_with_restart;

  TimerGroup group{&time_source};

  void setup()
  {
    mock().strictOrder();
  }

  static void callback(Timer * timer, TimerGroup * group)
  {
    mock()
      .actualCall("callback")
      .withParameter("group", group)
      .withParameter("context", timer);
  }

  static void callback_with_restart(Timer * timer, TimerGroup * group)
  {
    callback(timer, group);
    group->start(timer, restart_ticks, timer, callback_with_restart);
  }

  void after_timer_with_restart_is_started(Timer * timer, TimerTicks ticks)
  {
    restart_ticks = ticks;
    group.start(timer, ticks, timer, callback_with_restart);
  }

  void given_that_timer_with_restart_has_been_started(Timer * timer, TimerTicks ticks)
  {
    after_timer_with_restart_is_started(timer, ticks);
  }

  void after_timer_is_started(Timer * timer, TimerTicks ticks)
  {
    group.start(timer, ticks, timer, callback);
  }

  void given_that_timer_has_been_started(Timer * timer, TimerTicks ticks)
  {
    after_timer_is_started(timer, ticks);
  }

  void after_timer_is_stopped(Timer * timer)
  {
    group.stop(timer);
  }

  void given_that_timer_has_been_stopped(Timer * timer)
  {
    after_timer_is_stopped(timer);
  }

  void should_invoke_timer_callback(Timer * timer)
  {
    mock()
      .expectOneCall("callback")
      .withParameter("group", &group)
      .withParameter("context", timer);
  }

  void after(ITimeSource::TickCount ticks)
  {
    time_source.tick(ticks);
  }

  void after_the_group_is_run()
  {
    group.run();
  }

  void after_time_passes_and_the_group_is_run(ITimeSource::TickCount ticks)
  {
    after(ticks);
    after_the_group_is_run();
  }

  void given_that_time_has_passed_and_the_group_has_been_run(ITimeSource::TickCount ticks)
  {
    mock().disable();
    after_time_passes_and_the_group_is_run(ticks);
    mock().enable();
  }

  void should_invoke_timer_callback_after(Timer * timer, TimerTicks ticks)
  {
    after_time_passes_and_the_group_is_run(ticks - 1);
    should_invoke_timer_callback(timer);
    after_time_passes_and_the_group_is_run(1);
  }

  void should_run_and_indicate_that_the_next_timer_will_be_ready_in(TimerTicks ticks)
  {
    mock().disable();
    CHECK_EQUAL(ticks, group.run());
    mock().enable();
  }

  void timer_should_not_be_running(Timer * timer)
  {
    CHECK_FALSE(group.is_running(timer));
  }

  void timer_should_be_running(Timer * timer)
  {
    CHECK_TRUE(group.is_running(timer));
  }

  void remaining_timer_for_timer_should_be(Timer * timer, TimerTicks ticks)
  {
    CHECK_EQUAL(ticks, group.remaining_ticks(timer));
  }

  void nothing_should_happen()
  {
  }
};

TEST(TimerGroup, should_invoke_timer_callback_on_expiration)
{
  given_that_timer_has_been_started(&timer_1, 7);
  should_invoke_timer_callback_after(&timer_1, 7);
}

TEST(TimerGroup, should_invoke_timer_callback_even_if_run_after_expiration)
{
  given_that_timer_has_been_started(&timer_1, 7);

  after(10);
  should_invoke_timer_callback(&timer_1);
  after_the_group_is_run();
}

TEST(TimerGroup, should_not_call_back_timer_again_after_expiration)
{
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(7);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(100);
}

TEST(TimerGroup, should_allow_a_timer_to_be_stopped_prior_to_expiration)
{
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(3);
  given_that_timer_has_been_stopped(&timer_1);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(TimerGroup, should_allow_a_running_timer_to_be_restarted)
{
  given_that_timer_has_been_started(&timer_2, 7);
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_3, 9);
  given_that_timer_has_been_started(&timer_1, 5);

  should_invoke_timer_callback_after(&timer_1, 5);
  should_invoke_timer_callback_after(&timer_2, 2);
  should_invoke_timer_callback_after(&timer_3, 2);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(TimerGroup, should_manage_multiple_timers_simultaneously)
{
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_2, 3);

  should_invoke_timer_callback_after(&timer_2, 3);
  should_invoke_timer_callback_after(&timer_1, 4);
}

TEST(TimerGroup, should_invoke_at_most_one_callback_per_run)
{
  given_that_timer_has_been_started(&timer_1, 7);
  given_that_timer_has_been_started(&timer_2, 7);

  after(7);
  should_invoke_timer_callback(&timer_1);
  after_the_group_is_run();

  should_invoke_timer_callback(&timer_2);
  after_the_group_is_run();
}

TEST(TimerGroup, should_indicate_whether_a_callback_was_invoked_during_run)
{
  given_that_timer_has_been_started(&timer_1, 3);
  given_that_timer_has_been_started(&timer_2, 5);
  given_that_timer_has_been_started(&timer_3, 5);

  should_run_and_indicate_that_the_next_timer_will_be_ready_in(3);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(2);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(1);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(2);

  after(2);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(0);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(0xFFFF);
}

TEST(TimerGroup, should_indicate_whether_a_timer_is_running)
{
  timer_should_not_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);

  after_timer_is_started(&timer_1, 5);
  timer_should_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  timer_should_not_be_running(&timer_1);
  timer_should_not_be_running(&timer_2);
}

TEST(TimerGroup, should_give_the_remaining_ticks_for_a_running_timer)
{
  given_that_timer_has_been_started(&timer_1, 7);
  remaining_timer_for_timer_should_be(&timer_1, 7);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  remaining_timer_for_timer_should_be(&timer_1, 2);
}

TEST(TimerGroup, should_allow_a_timer_to_be_restarted_in_its_callback)
{
  given_that_timer_with_restart_has_been_started(&timer_with_restart, 5);

  should_invoke_timer_callback_after(&timer_with_restart, 5);
  should_invoke_timer_callback_after(&timer_with_restart, 5);
  should_invoke_timer_callback_after(&timer_with_restart, 5);
}

TEST(TimerGroup, should_not_allow_a_timer_to_be_starved)
{
  given_that_timer_with_restart_has_been_started(&timer_with_restart, 0);
  given_that_timer_has_been_started(&timer_1, 3);

  should_invoke_timer_callback(&timer_with_restart);
  after_time_passes_and_the_group_is_run(3);

  should_invoke_timer_callback(&timer_1);
  after_the_group_is_run();
}
