CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Examples
  * Installation
  * Known Issues/Shortcomings
  * Maintainers


INTRODUCTION
------------
Relationships or other joins in Views often create "duplicate" results. For
example, a node with a field that has multiple values may show up in the View
once per value in the multi-value field. It's frustrating, and the "DISTINCT"
option in the Views UI does not actually solve the problem because the result
row is technically distinct.

This module aims to give a simple GUI method to remove or aggregate these
"duplicate" rows. For any given field, including "Global: Text" fields, you can
optionally mark the field as filtered ("Filter Repeats") or aggregated
("Aggregate Repeats"). All rows with the same value in that field will either be
removed as duplicates (filtered), or aggregated in-line.

The "value" of the field as used for filtering or aggregation can be taken
pre-render (fastest and totally cacheable), or post-render (after any rewrite
rules or other transformations have occurred).  Post-render actions are a bit
slower (the View must be re-rendered, though the query is not re-run), and
currently Output caching is unsupported for post-rendered changes.  You can
still use query caching, however.  Post-render checks also work with Global
fields, like Global: Text w/rewrite rules.

EXAMPLES
--------
Consider a Course node with multiple Instructor fields:

    1) Course title: CHEM 101 - Introduction to Chemistry
       Instructor: Mr. Smith
    2) Course title: CHEM 101 - Introduction to Chemistry
       Instructor: Ms. Jones

when Aggregating on the Instructor field, and Filtering on the Course Title
field, the resulting view could like like:

    1) Course title: CHEM 101 - Introduction to Chemistry
       Instructor(s): Mr. Smith, Ms. Jones

Or, if there were multiple Course nodes (say, for multiple terms) the View may
by default be:

    1) Course title: CHEM 101 - Introduction to Chemistry
       Instructor: Mr. Smith
       Term: Fall 2013
    2) Course title: CHEM 101 - Introduction to Chemistry
       Instructor: Ms. Jones
       Term: Winter 2013

when Filtering on the Course Title field, the view could look like:

    1) Course title: CHEM 101 - Introduction to Chemistry

(note, you may want to remove the Term field, since it no longer applies once
we're purposely removing multiple term rows from the results)

Or, Aggregating on both Instructor and Term fields:

    1) Course title: CHEM 101 - Introduction to Chemistry
       Instructor(s): Mr. Smith, Ms. Jones
       Term(s): Fall 2013, Winter 2013


INSTALLATION
------------
Activate the module, then administer a desired View. Under any field you want
to affect, Edit the field and select the appropriate Aggregate or Filter option
under the "Views Distinct Settings" section of the configuration form.

If you don't have have a good field to disambiguate "duplicate" rows, you can
add a Global: Text and rewrite it with some combination of existing fields,
like the rewrite values for a course title display:
"[class_subject] [class_number] [class_title]". Be sure to enable the
post-rendering option, or rewrites will not work!


KNOWN ISSUES/SHORTCOMINGS
-------------------------
These are on the To-Do list, but don't seem critical enough to prevent this
module from helping a lot of people. Still, they may cause odd behavior, so
it's best if folks know about them:

    1) Pager counts and the number of rows displayed are incorrect when
      filtering (removing) duplicates, and aggregation cannot aggregate fields
      from outside the scope of each page, since each page only has access to
      the rows on that page.  This is a known issue without a fix for now.
      Results won't be scrambled, but fewer-than-expected results may show up
      on pagers; please test the outcome and choose if the pager is worth the
      oddness.
    2) Output (HTML) caching is not supported when the field is
      Filtered/Aggregated on its rendered value. If output caching is enabled
      on the View, don't enable post-render filtering/aggregation. This does
      not affect Views query caching, which works.
    3) Filtering out results does not update the pager, even in
      hook_views_post_execute() when unset()ing entries $view->results.

MAINTAINERS
-----------
- jay.dansand (Jay Dansand)
