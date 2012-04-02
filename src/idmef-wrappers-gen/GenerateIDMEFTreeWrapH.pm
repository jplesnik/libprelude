# Copyright (C) 2003, 2004, 2005 PreludeIDS Technologies. All Rights Reserved.
# Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
#
# This file is part of the Prelude library.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

package GenerateIDMEFTreeWrapH;

use Generate;
@ISA = qw/Generate/;

use strict;
use IDMEFTree;

sub     header
{
    my  $self = shift;

    $self->output("
/*****
*
* Copyright (C) 2001, 2002, 2003, 2004, 2005 PreludeIDS Technologies. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v\@prelude-ids.com>
* Author: Nicolas Delon <nicolas.delon\@prelude-ids.com>
*
* This file is part of the Prelude library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*****/

/* Auto-generated by the GenerateIDMEFTreeWrapH package */

#ifndef _LIBPRELUDE_IDMEF_TREE_WRAP_H
#define _LIBPRELUDE_IDMEF_TREE_WRAP_H

#include <limits.h>
#include \"idmef-class.h\"
#include \"idmef-value.h\"
#include \"prelude-inttypes.h\"
#include \"prelude-string.h\"
#include \"prelude-msg.h\"

#ifdef __cplusplus
  extern \"C\" \{
#endif

#ifdef WIN32
# undef interface
#endif

#define IDMEF_LIST_APPEND  INT_MAX
#define IDMEF_LIST_PREPEND (INT_MAX - 1)

");
}

sub     footer
{
    my  $self = shift;

    $self->output("
void idmef_message_set_pmsg(idmef_message_t *message, prelude_msg_t *msg);

prelude_msg_t *idmef_message_get_pmsg(idmef_message_t *message);

#ifdef __cplusplus
  }
#endif
");

    $self->output("#define $_->[0] $_->[1]\n") foreach ( @{ $self->{type_list} } );
    $self->output("\n");
    $self->output("#endif /* _LIBPRELUDE_IDMEF_TREE_WRAP */\n");
}

sub     struct_definition
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    $self->output("\n\n/*\n");
    $self->output(" * $_\n") foreach ( @{ $struct->{desc} } );
    $self->output(" */\n\n");
    $self->output("
typedef struct idmef_$struct->{short_typename} $struct->{typename};

") unless ( $tree->{pre_declareds}->{$struct->{typename}} );
}

sub     struct_constructor
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    $self->output("int idmef_$struct->{short_typename}_new($struct->{typename} **ret);\n");
    $self->output("int idmef_$struct->{short_typename}_copy(const $struct->{typename} *src, $struct->{typename} *dst);\n");
    $self->output("int idmef_$struct->{short_typename}_clone($struct->{typename} *src, $struct->{typename} **dst);\n");
    $self->output("int idmef_$struct->{short_typename}_compare(const $struct->{typename} *obj1, const $struct->{typename} *obj2);\n");
}

sub     struct_ref
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    $struct->{refcount} or return;

    $self->output("$struct->{typename} *idmef_$struct->{short_typename}_ref($struct->{typename} *$struct->{short_typename});\n");

}

sub     struct_destroy_child
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $n = 0;

    $self->output("int _idmef_$struct->{short_typename}_destroy_child(void *p, idmef_class_child_id_t child, int n);\n");
}

sub     struct_get_child
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $n = 0;

    $self->output("int _idmef_$struct->{short_typename}_get_child(void *p, idmef_class_child_id_t child, void **childptr);\n");
}

sub     struct_new_child
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $n = 0;

    $self->output("int _idmef_$struct->{short_typename}_new_child(void *p, idmef_class_child_id_t child, int n, void **ret);\n");
}

sub     struct_destroy
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    $self->output("void idmef_$struct->{short_typename}_destroy($struct->{typename} *ptr);\n");
}

sub     struct_field_normal
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $field = shift;
    my  $name = shift || $field->{name};
    my  $ptr = "";
    my  $refer = "";

    $ptr = "*", $refer = "*" if ( $field->{metatype} & &METATYPE_STRUCT );
    $refer = "*" if ( $field->{metatype} & &METATYPE_OPTIONAL_INT );

    if ( $field->{metatype} & &METATYPE_OPTIONAL_INT ) {
        $self->output("void idmef_$struct->{short_typename}_unset_${name}($struct->{typename} *ptr);\n");
    }

    $self->output("$field->{typename} ${refer}idmef_$struct->{short_typename}_get_${name}($struct->{typename} *ptr);\n");

    my $field_name = ($name eq "class") ? "class_str" : $name;

    $self->output("void idmef_$struct->{short_typename}_set_${name}($struct->{typename} *ptr, $field->{typename} ${ptr}${field_name});\n");

    $self->output("int idmef_$struct->{short_typename}_new_${name}($struct->{typename} *ptr, $field->{typename} **ret);\n");

    $self->output("\n");
}

sub     struct_field_union
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $field = shift;

    $self->output("$field->{typename} idmef_$struct->{short_typename}_get_$field->{var}($struct->{typename} *ptr);\n");

    foreach my $member ( @{ $field->{member_list} } ) {
        $self->struct_field_normal($tree, $struct,
                                   { metatype => &METATYPE_STRUCT,
                                     name => "$field->{name}.$member->{name}",
                                     typename => $member->{typename},
                                     short_typename => $member->{short_typename},
                                     ptr => $member->{ptr}
                                   }, $member->{name});
    }
}

sub     struct_field_list
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;
    my  $field = shift;

    $self->output(
"$field->{typename} *idmef_$struct->{short_typename}_get_next_$field->{short_name}($struct->{typename} *$struct->{short_typename}, $field->{typename} *$field->{short_typename}_cur);
void idmef_$struct->{short_typename}_set_$field->{short_name}($struct->{typename} *ptr, $field->{typename} *object, int pos);
int idmef_$struct->{short_typename}_new_$field->{short_name}($struct->{typename} *ptr, $field->{typename} **ret, int pos);

");
}

sub     struct_fields
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    foreach my $field ( @{ $struct->{field_list} } ) {
        $self->struct_field_normal($tree, $struct, $field) if ( $field->{metatype} & &METATYPE_NORMAL );
        $self->struct_field_list($tree, $struct, $field) if ( $field->{metatype} & &METATYPE_LIST );
        $self->struct_field_union($tree, $struct, $field) if ( $field->{metatype} & &METATYPE_UNION );
    }
}

sub     struct
{
    my  $self = shift;
    my  $tree = shift;
    my  $struct = shift;

    $self->struct_definition($tree, $struct);
    $self->struct_constructor($tree, $struct);
    $self->struct_ref($tree, $struct);

    $self->output("\n#ifndef SWIG\n");
    $self->struct_get_child($tree, $struct);
    $self->struct_new_child($tree, $struct);
    $self->struct_destroy_child($tree, $struct);
    $self->output("#endif\n\n");

    $self->struct_destroy($tree, $struct);
    $self->struct_fields($tree, $struct);
}

sub     enum
{
    my  $self = shift;
    my  $tree = shift;
    my  $enum = shift;
    my  $cnt = 0;

    foreach ( @{ $enum->{desc} } ) {
        if ( $cnt == 0 ) {
            $self->output("typedef enum {\n");

        } elsif ( $cnt + 1 == @{ $enum->{desc} } ) {
            $self->output("\} $enum->{typename};\n");

        } else {
            my $val = $_;

            $val =~ s/\(.*\)//;
            $self->output("$val\n");
        }

        $cnt++;
    }

    $self->output("\n\n");
    $self->output("$enum->{typename} idmef_$enum->{short_typename}_to_numeric(const char *name);", "\n");
    $self->output("const char *idmef_$enum->{short_typename}_to_string($enum->{typename} val);", "\n");
    $self->output("\n\n");
}

sub     pre_declared
{
    my  $self = shift;
    my  $tree = shift;
    my  $pre_declared = shift;

    if ( $pre_declared->{metatype} & &METATYPE_STRUCT ) {
    $self->output("
typedef struct idmef_$pre_declared->{short_typename} $pre_declared->{typename};

");
    } else {
    $self->output("
typedef enum idmef_$pre_declared->{short_typename} $pre_declared->{typename};

");
    }
}

sub     obj
{
    my  $self = shift;
    my  $tree = shift;
    my  $obj = shift;

    push(@{ $self->{type_list} }, [ "IDMEF_CLASS_ID_" . uc("$obj->{short_typename}") => $obj->{id} ])
        if ( $obj->{obj_type} != &OBJ_PRE_DECLARED );
}

1;
