/*!
 * @file
 * @brief
 */

#include "tiny/Timer.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "double/TimeSourceDouble.h"

using namespace tiny;

static TimerTicks restart_ticks;
static TimerGroup* static_group;

TEST_GROUP(Timer)
{
  TimeSourceDouble time_source{};

  Timer timer_1{};
  Timer timer_2{};
  Timer timer_3{};
  Timer timer_with_restart{};

  TimerGroup group{time_source};

  void setup()
  {
    mock().strictOrder();

    static_group = &group;
  }

  static void callback(Timer * timer)
  {
    mock()
      .actualCall("callback")
      .withParameter("context", timer);
  }

  static void callback_with_restart(Timer * timer)
  {
    callback(timer);
    static_group->start(*timer, restart_ticks, timer, callback_with_restart);
  }

  void after_timer_with_restart_is_started(Timer & timer, TimerTicks ticks)
  {
    restart_ticks = ticks;
    group.start(timer, ticks, &timer, callback_with_restart);
  }

  void given_that_timer_with_restart_has_been_started(Timer & timer, TimerTicks ticks)
  {
    after_timer_with_restart_is_started(timer, ticks);
  }

  static void callback_with_stop(Timer * timer)
  {
    callback(timer);
    static_group->stop(*timer);
  }

  void given_that_periodic_timer_with_stop_has_been_started(Timer & timer, TimerTicks ticks)
  {
    group.start_periodic(timer, ticks, &timer, callback_with_stop);
  }

  static void callback_with_periodic_restart(Timer * timer)
  {
    callback(timer);
    static_group->start_periodic(*timer, restart_ticks, timer, callback);
  }

  void given_that_periodic_timer_with_restart_has_been_started(Timer & timer, TimerTicks ticks, TimerTicks _restart_ticks)
  {
    restart_ticks = _restart_ticks;
    group.start_periodic(timer, ticks, &timer, callback_with_periodic_restart);
  }

  void after_timer_is_started(Timer & timer, TimerTicks ticks)
  {
    group.start(timer, ticks, &timer, callback);
  }

  void given_that_timer_has_been_started(Timer & timer, TimerTicks ticks)
  {
    after_timer_is_started(timer, ticks);
  }

  void after_periodic_timer_is_started(Timer & timer, TimerTicks ticks)
  {
    group.start_periodic(timer, ticks, &timer, callback);
  }

  void given_that_periodic_timer_has_been_started(Timer & timer, TimerTicks ticks)
  {
    after_periodic_timer_is_started(timer, ticks);
  }

  void after_timer_is_stopped(Timer & timer)
  {
    group.stop(timer);
  }

  void given_that_timer_has_been_stopped(Timer & timer)
  {
    after_timer_is_stopped(timer);
  }

  void should_invoke_timer_callback(Timer & timer)
  {
    mock()
      .expectOneCall("callback")
      .withParameter("context", &timer);
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

  void should_invoke_timer_callback_after(Timer & timer, TimerTicks ticks)
  {
    after_time_passes_and_the_group_is_run(ticks - 1);
    should_invoke_timer_callback(timer);
    after_time_passes_and_the_group_is_run(1);
  }

  void should_run_and_indicate_that_the_next_timer_will_be_ready_in(TimerTicks expected)
  {
    mock().disable();
    auto actual = group.run();
    CHECK_EQUAL(expected, actual);
    mock().enable();
  }

  void timer_should_not_be_running(Timer & timer)
  {
    CHECK_FALSE(group.is_running(timer));
  }

  void timer_should_be_running(Timer & timer)
  {
    CHECK_TRUE(group.is_running(timer));
  }

  void remaining_timer_for_timer_should_be(Timer & timer, TimerTicks ticks)
  {
    CHECK_EQUAL(ticks, group.remaining_ticks(timer));
  }

  void nothing_should_happen()
  {
  }
};

TEST(Timer, should_invoke_timer_callback_on_expiration)
{
  given_that_timer_has_been_started(timer_1, 7);
  should_invoke_timer_callback_after(timer_1, 7);
}

TEST(Timer, should_invoke_timer_callback_even_if_run_after_expiration)
{
  given_that_timer_has_been_started(timer_1, 7);

  after(10);
  should_invoke_timer_callback(timer_1);
  after_the_group_is_run();
}

TEST(Timer, should_not_call_back_timer_again_after_expiration)
{
  given_that_timer_has_been_started(timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(7);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(100);
}

TEST(Timer, should_allow_a_timer_to_be_stopped_prior_to_expiration)
{
  given_that_timer_has_been_started(timer_1, 7);
  given_that_time_has_passed_and_the_group_has_been_run(3);
  given_that_timer_has_been_stopped(timer_1);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(Timer, should_allow_a_running_timer_to_be_restarted)
{
  given_that_timer_has_been_started(timer_2, 7);
  given_that_timer_has_been_started(timer_1, 7);
  given_that_timer_has_been_started(timer_3, 9);
  given_that_timer_has_been_started(timer_1, 5);

  should_invoke_timer_callback_after(timer_1, 5);
  should_invoke_timer_callback_after(timer_2, 2);
  should_invoke_timer_callback_after(timer_3, 2);

  nothing_should_happen();
  after_time_passes_and_the_group_is_run(10);
}

TEST(Timer, should_manage_multiple_timers_simultaneously)
{
  given_that_timer_has_been_started(timer_1, 7);
  given_that_timer_has_been_started(timer_2, 3);

  should_invoke_timer_callback_after(timer_2, 3);
  should_invoke_timer_callback_after(timer_1, 4);
}

TEST(Timer, should_run_periodic_timers)
{
  given_that_periodic_timer_has_been_started(timer_1, 7);

  should_invoke_timer_callback_after(timer_1, 7);
  should_invoke_timer_callback_after(timer_1, 7);
}

TEST(Timer, should_allow_periodic_timers_to_be_stopped_from_their_callback)
{
  given_that_periodic_timer_with_stop_has_been_started(timer_1, 7);

  should_invoke_timer_callback_after(timer_1, 7);

  nothing_should_happen();
  after(7);
}

TEST(Timer, should_allow_periodic_timers_to_have_period_changed_in_callback)
{
  given_that_periodic_timer_with_restart_has_been_started(timer_1, 7, 5);

  should_invoke_timer_callback_after(timer_1, 7);
  should_invoke_timer_callback_after(timer_1, 5);
  should_invoke_timer_callback_after(timer_1, 5);
}

TEST(Timer, should_invoke_at_most_one_callback_per_run)
{
  given_that_timer_has_been_started(timer_1, 7);
  given_that_timer_has_been_started(timer_2, 7);

  after(7);
  should_invoke_timer_callback(timer_1);
  after_the_group_is_run();

  should_invoke_timer_callback(timer_2);
  after_the_group_is_run();
}

TEST(Timer, should_indicate_whether_a_callback_was_invoked_during_run)
{
  given_that_timer_has_been_started(timer_1, 3);
  given_that_timer_has_been_started(timer_2, 5);
  given_that_timer_has_been_started(timer_3, 5);

  should_run_and_indicate_that_the_next_timer_will_be_ready_in(3);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(2);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(1);

  after(1);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(2);

  after(2);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(0);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(0xFFFFFFFF);
}

TEST(Timer, should_account_for_periodic_timers_when_giving_time_until_next_ready)
{
  given_that_periodic_timer_has_been_started(timer_1, 2);
  given_that_timer_has_been_started(timer_2, 7);

  after(2);
  should_run_and_indicate_that_the_next_timer_will_be_ready_in(2);
}

TEST(Timer, should_indicate_whether_a_timer_is_running)
{
  timer_should_not_be_running(timer_1);
  timer_should_not_be_running(timer_2);

  after_timer_is_started(timer_1, 5);
  timer_should_be_running(timer_1);
  timer_should_not_be_running(timer_2);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  timer_should_not_be_running(timer_1);
  timer_should_not_be_running(timer_2);
}

TEST(Timer, should_give_the_remaining_ticks_for_a_running_timer)
{
  given_that_timer_has_been_started(timer_1, 7);
  remaining_timer_for_timer_should_be(timer_1, 7);

  given_that_time_has_passed_and_the_group_has_been_run(5);
  remaining_timer_for_timer_should_be(timer_1, 2);
}

TEST(Timer, should_allow_a_timer_to_be_restarted_in_its_callback)
{
  given_that_timer_with_restart_has_been_started(timer_with_restart, 5);

  should_invoke_timer_callback_after(timer_with_restart, 5);
  should_invoke_timer_callback_after(timer_with_restart, 5);
  should_invoke_timer_callback_after(timer_with_restart, 5);
}

TEST(Timer, should_not_allow_a_timer_to_be_starved)
{
  given_that_timer_with_restart_has_been_started(timer_with_restart, 0);
  given_that_timer_has_been_started(timer_1, 3);

  should_invoke_timer_callback(timer_with_restart);
  after_time_passes_and_the_group_is_run(3);

  should_invoke_timer_callback(timer_1);
  after_the_group_is_run();
}
