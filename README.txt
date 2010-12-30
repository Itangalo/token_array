// $Id: README.txt,v 1.1.2.2 2010/12/30 09:25:18 itangalo Exp $

The Array Tokens module allows you to use all values in multiple-value CCK
fields and multiple-value taxonomy terms in a single token, like this:

  [node:terms-1-default-array]
    becomes "tag1, tag2, tag3"

  [node:field_nodereference-default-array-view]
    becomes "nodeone, nodetwo, nodethree"

It also allows you to define your own settings for how items should be merged
("merge styles"). This is done at admin/settings/token_array and requires the
configure site settings permission. The effect could look like this:

  [node:terms-1-twitter-array] becomes "#tag1 #tag2 #tag3"

The output of each item is either plain text (for taxonomy terms) or the default
CCK rendered output (for CCK fields). However, a few special formats are
provided for file fields, node references and user references. Like this:

  [node:field_file-default-array-fid]
  [node:field_nodereference-default-array-nid]
  [node:field_userreference-default-array-uid]
    ...become "1, 2, 3"


INSTALLING ARRAY TOKENS
-----------------------

Array Tokens is installed in the plain vanilla way.

* Download the module, extract it into sites/all/modules (or any other sub
  folder of sites/ if you can motivate it).

* Visit your module list and enable Array Tokens.


USING ARRAY TOKENS
------------------

New tokens will appear in token lists where node objects AND multiple-value
fields/terms are available. Use as any token.


CONFIGURING ARRAY TOKENS
------------------------

If you're not satisfied with the merge styles included with the installation,
you are free to configure your own. Add as many merge styles you wish at
admin/settings/token_array. Note that many merge styles will probably lead to
a long token list. More importantly, note that you are allowed to enter markup
in prefix, suffix and infix. This means that any user allowed to configure Array
Tokens potentially have the power to break the markup on your site.


EXPORTING ARRAY TOKENS
----------------------

The merge styles are stored as a Drupal system variable, meaning that it can be
exported with Features using the Strongarm module. At this point there are no
plans on providing separate export functionality. (Sorry.)


CONTRIBUTING TO ARRAY TOKENS
----------------------------

You can contribute to the Array Tokens project! Bug reports, feature requests,
documentation, translations, patches, ports to Drupal 7 or just general cheering
and hand clapping is always welcome.

Go to http://drupal.org/project/token_array to find out more.
