Evergreen Test Automation
=============

The Evergreen test automation reduces the overhead of running the test cases
required to test Evergreen-specific functionality. These tests must be run both
internally and by partners, and making them simple and fast is good for all.

Structure
=============

The Evergreen test automation framework is made up of a few distinct sets of
files.

**Core Scripts**

These are files that are responsible for finding all of the tests, executing all
of the tests, and outputting the results.

* `run_all_tests.sh`
* `setup.sh`
* `pprint.sh`

**Shared Scripts**

These files contain code that is either non-trivial or repeated throughout the
tests, and is shared across all platforms.

* `shared/app_key.sh`
* `shared/cycle_cobalt.sh`
* `shared/drain_file.sh`
* `shared/init_logging.sh`
* `shared/installation_slot.sh`
* `shared/wait_and_watch.sh`

**Platform-Specific Scripts**

These files contain code that is either non-trivial or repeated throughout the
tests, and is platform specific.

* `<PLATFORM>/clean_up.sh`
* `<PLATFORM>/clear_storage.sh`
* `<PLATFORM>/create_file.sh`
* `<PLATFORM>/delete_file.sh`
* `<PLATFORM>/deploy_cobalt.sh`
* `<PLATFORM>/run_command.sh`
* `<PLATFORM>/setup.sh`
* `<PLATFORM>/start_cobalt.sh`
* `<PLATFORM>/stop_cobalt.sh`

**Test HTML**

These files are responsible for changing the channels when tests are running.

* `tests/empty.html`
* `tests/test.html`
* `tests/tseries.html`

**Test Cases**

These files are responsible for the test logic, and each file corresponds to a
single Evergreen test case.

* `tests/abort_update_if_already_updating_test.sh`
* `tests/alternative_content_test.sh`
* `tests/continuous_updates_test.sh`
* `tests/crashing_binary_test.sh`
* `tests/crashpad_runs_test.sh`
* `tests/evergreen_lite_test.sh`
* `tests/load_slot_being_updated_test.sh`
* `tests/mismatched_architecture_test.sh`
* `tests/noop_binary_test.sh`
* `tests/out_of_storage_test.sh`
* `tests/quick_roll_forward_test.sh`
* `tests/racing_updaters_test.sh`
* `tests/update_fails_verification_test.sh`
* `tests/update_works_for_only_one_app_test.sh`
* `tests/valid_slot_overwritten_test.sh`
* `tests/verify_qa_channel_compressed_update_test.sh`
* `tests/verify_qa_channel_update_test.sh`

How To Run
=============

Before beginning, please check if your target platform has a README.md and defer
to the steps to run specified there. Otherwise, there are two primary methods of
running the Evergreen tests.

**Python Helper Script**

The Python helper script at `cobalt/evergreen_tests/evergreen_tests.py`
simplifies the process of running the automated tests, and relies on the
existing abstract launcher infrastructure.

For this example we will use the following:

* `linux-x64x11` and `qa` for the target platform and configuration.
* `evergreen-x64` and `qa` for the Evergreen platform and configuration.

Then the following command can be used to run the tests.

```
python cobalt/evergreen_tests/evergreen_tests.py \
    -p evergreen-x64 -c qa -P linux-x64x11 -C qa
```

**Directly**

First, a directory tree containing the required binaries and content needs to be
created. This directory tree must be in the following format:

Directly running the scripts requires more setup than the helper script above.
We will be using the same platforms and configurations as the steps above.

First, a directory structure needs to be created where the Evergreen binary and
its content is located under the content of the loader binary's content:

```
  linux-x64x11_qa
   +-- deploy
        +-- loader_app
             +-- loader_app <-- loader binary
             +-- content    <-- loader content
                  +-- app
                       +-- cobalt
                            +-- content                 <-- cobalt content
                            +-- lib
                                 +-- libcobalt.{so,lz4} <-- cobalt binary
```

Note: This directory structure is the same as what would be generated by
      `starboard/evergreen/shared/launcher.py`.

Next, set the environment variable `OUT` equal to the root of the directory tree
created above.

Then the following command can be used to run the tests.

```
  ./run_all_test.sh linux
```

Tips
=============

The tests will take between 15 and 30 minutes to complete and generate a
significant amount of logs. The tests can be run in the background with the logs
redirected using `./run_all_tests.sh linux &> results &`.

When redirecting all of the logs from the test script being output to `results`,
you can easily check the status by running `grep -E "RUN|PASSED|FAILED"`.

Notes
=============

Evergreen uses "drain" files to ensure only one application downloads an update
at a time. To fake update contention, some tests create this file manually:

* `tests/abort_update_if_already_updating_test.sh`
* `tests/load_slot_being_updated_test.sh`
* `tests/racing_updaters_test.sh`

Evergreen uses "app key" files to keep track of per-application state of an
update. To fake changes to the per-application state of an update, some tests
create, delete, or modify these files manually:

* `tests/update_works_for_only_one_app_test.sh`
* `tests/valid_slot_overwritten_test.sh`

To validate Evergreen behavior when there is not enough storage for an update, a
temporary filesystem is used, only 10MiB in size. One test creates a symbolic
link from the storage path to this filesystem, faking an "out of storage"
situation:

* `tests/out_of_storage_test.sh`