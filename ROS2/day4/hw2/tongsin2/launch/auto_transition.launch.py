#공식 ROS2 문서참고

import launch
import launch.actions
import launch.events
import launch.event_handlers.on_process_start

import launch_ros.actions
import launch_ros.events
import launch_ros.events.lifecycle
import launch_ros.event_handlers

import lifecycle_msgs.msg

def generate_launch_description():
    talker_node = launch_ros.actions.LifecycleNode(
                package='tongsin2',
                executable='my_publisher',
                name='my_publisher',
                namespace='',
                output='screen'
    )
            
    
    listener_node = launch_ros.actions.LifecycleNode(
                package='tongsin2',
                executable='my_subscriber',
                name='my_subscriber',
                namespace='',
                output='screen'
    )

    return launch.LaunchDescription([
        talker_node, listener_node,
        # Right after the talker starts, make it take the 'configure' transition.
        launch.actions.RegisterEventHandler(
            launch.event_handlers.on_process_start.OnProcessStart(
                target_action=talker_node,
                on_start=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(talker_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,
                    )),
                ],
            )
        ),
        # When the talker reaches the 'inactive' state, make it take the 'activate' transition.
        launch.actions.RegisterEventHandler(
            launch_ros.event_handlers.OnStateTransition(
                target_lifecycle_node=talker_node,
                start_state='configuring', goal_state='inactive',
                entities=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(talker_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,
                    )),
                ],
            )
        ),
        # When the talker node reaches the 'active' state, wait a bit and then make it take the
        # 'deactivate' transition.
        launch.actions.RegisterEventHandler(
            launch_ros.event_handlers.OnStateTransition(
                target_lifecycle_node=talker_node, start_state='activating', goal_state='active',
                entities=[
                    launch.actions.TimerAction(period=5.0, actions=[
                        launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                            lifecycle_node_matcher=launch.events.matches_action(talker_node),
                            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_DEACTIVATE,
                        )),
                    ]),
                ],
            )
        ),
        # When the talker node reaches the 'inactive' state coming from the 'active' state,
        # make it take the 'cleanup' transition.
        launch.actions.RegisterEventHandler(
            launch_ros.event_handlers.OnStateTransition(
                target_lifecycle_node=talker_node,
                start_state='deactivating', goal_state='inactive',
                entities=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(talker_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CLEANUP,
                    )),
                ],
            )
        ),
        # When the talker node reaches the 'unconfigured' state after a 'cleanup' transition,
        # make it take the 'unconfigured_shutdown' transition.
        launch.actions.RegisterEventHandler(
            launch_ros.event_handlers.OnStateTransition(
                target_lifecycle_node=talker_node,
                start_state='cleaningup', goal_state='unconfigured',
                entities=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(talker_node),
                        transition_id=(
                            lifecycle_msgs.msg.Transition.TRANSITION_UNCONFIGURED_SHUTDOWN
                        ),
                    )),
                ],
            )
        ),
                # listener configure
        launch.actions.RegisterEventHandler(
            launch.event_handlers.on_process_start.OnProcessStart(
                target_action=listener_node,
                on_start=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(listener_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,
                    )),
                ],
            )
        ),
        # listener activate
        launch.actions.RegisterEventHandler(
            launch_ros.event_handlers.OnStateTransition(
                target_lifecycle_node=listener_node,
                start_state='configuring', goal_state='inactive',
                entities=[
                    launch.actions.EmitEvent(event=launch_ros.events.lifecycle.ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(listener_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,
                    )),
                ],
            )
        )
    ])


