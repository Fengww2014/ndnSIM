# -*- coding: utf-8 -*-
#
# NFD documentation build configuration file, created by
# sphinx-quickstart on Wed Mar  7 19:23:56 2018.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

import datetime
import os
import sys

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
sys.path.insert(0, os.path.abspath('.'))


# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.todo',
    'redmine_issue'
]

def addExtensionIfExists(extension):
    try:
        __import__(extension)
        extensions.append(extension)
    except ImportError:
        sys.stderr.write("Extension '%s' in not available. "
                         "Some documentation may not build correctly.\n" % extension)
        sys.stderr.write("To install, use \n"
                         "  sudo pip install %s\n" % extension.replace('.', '-'))

if sys.version_info[0] >= 3:
    addExtensionIfExists('sphinxcontrib.doxylink')

# sphinxcontrib.googleanalytics is currently not working with the latest version of Sphinx
# if os.getenv('GOOGLE_ANALYTICS', None):
#     addExtensionIfExists('sphinxcontrib.googleanalytics')

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'NFD - Named Data Networking Forwarding Daemon'
copyright = u'2014-%d, Named Data Networking Project' % datetime.date.today().year
author = u'Named Data Networking Project'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
# version = u''
# The full version, including alpha/beta/rc tags.
# release = u''

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
# html_theme = 'alabaster'
html_theme = 'named_data_theme'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
html_theme_path = ['.']

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


# -- Options for HTMLHelp output ------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'nfd-docs'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    ('index', 'nfd-docs.tex', u'NFD - Named Data Networking Forwarding Daemon Documentation',
     u'Named Data Networking Project', 'manual'),
]


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    ('manpages/nfd', 'nfd', u'Named Data Networking Forwarding Daemon', '', 1),
    ('manpages/nfdc', 'nfdc', u'Interact with NFD management', '', 1),
    ('manpages/nfdc-status', 'nfdc-status', u'Show NFD status', '', 1),
    ('manpages/nfdc-face', 'nfdc-face', u'Show and manipulate NFD faces', '', 1),
    ('manpages/nfdc-route', 'nfdc-route', u'Show and manipulate NFD routes', '', 1),
    ('manpages/nfdc-cs', 'nfdc-cs', u'Show and manipulate NFD Content Store', '', 1),
    ('manpages/nfdc-strategy', 'nfdc-strategy', u'Show and manipulate NFD strategy choices', '', 1),
    ('manpages/nfd-status', 'nfd-status', u'Comprehensive report of NFD status', '', 1),
    ('manpages/nfd-status-http-server', 'nfd-status-http-server', u'NFD status HTTP server', '', 1),
    ('manpages/ndn-autoconfig-server', 'ndn-autoconfig-server', u'NDN auto-configuration server', '', 1),
    ('manpages/ndn-autoconfig', 'ndn-autoconfig', u'NDN auto-configuration client', '', 1),
    ('manpages/ndn-autoconfig.conf', 'ndn-autoconfig.conf', u'NDN auto-configuration client configuration file', '', 5),
    ('manpages/nfd-autoreg', 'nfd-autoreg', u'NFD auto-registration server', '', 1),
    ('manpages/nfd-asf-strategy', 'nfd-asf-strategy', u'NFD ASF strategy', '', 7),
]


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = []


doxylink = {
    'nfd' : ('NFD.tag', 'doxygen/'),
}

if os.getenv('GOOGLE_ANALYTICS', None):
    googleanalytics_id = os.environ['GOOGLE_ANALYTICS']
    googleanalytics_enabled = True

redmine_project_url = 'https://redmine.named-data.net/'