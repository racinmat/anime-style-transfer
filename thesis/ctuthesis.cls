%!TEX ROOT = ctutest.tex

\RequirePackage{expl3,l3keys2e}
\RequirePackage{environ}
\RequirePackage{xparse}
\RequirePackage[utf8]{inputenc}

\ProvidesExplClass{ctuthesis}%
	{2015/11/15}{0.1 t1511151022}%MY TIMESTAMP HERE {0.1}
	{Class for typesetting CTU theses and alike}

% Some general remarks: the module name for this class is `ctuthesis`. Yes, it is quite long,
% but at the very same time, it's descriptive.

% The core of the class.
\input{ctuth-core.tex}

% Process class keys (and call the trigger)
\ProcessKeysOptions { ctuthesis }
\ctuthesis_ctusetup_trigger:

% Load the report class
\LoadClass [
	a4paper, % pagesize option
	\bool_if:cT { g_ctuthesis_switch_draft_bool } { draft } , % draft option
	\bool_if:cTF { g_ctuthesis_switch_oneside_bool } { oneside } { twoside }, % one/twoside option
	\tl_use:N \g_ctuthesis_fontsize_tl % fontsize option
	] { book }

% Once the class is loaded, we disable keys marked 'nop'
\ctuthesis_disable_keys:n { nop }

% This is the macro that actually loads all the stuff. It ought to be used exactly in the preamble.
% The reason for this setup is that it allows \ctusetup parameters to influence things that need
% to be influenced.
\DeclareDocumentCommand \ctuprocess { } {
	% Get to both LaTeX2e and LaTeX3 context
	\makeatletter
	\ExplSyntaxOn
	% Load the two files
	\input{ctuth-pkg.tex}
	\input{ctuth-templates.tex}
	\ExplSyntaxOff
	\makeatother
	% Once \ctuprocess is called, we disable keys marked 'nop'
	\ctuthesis_disable_keys:n { nopost }
	% Make more calls to \ctuprocess lead to an error
	\let\ctuprocess\ctuprocess@postprocess
	% Disable check for missing \ctuprocess
	\let\ctuprocess@check\relax
}

% The two macros that only invoke an error -- used in checks whether \ctuprocess was called exactly once
\DeclareDocumentCommand \ctuprocess@postprocess { } { \msg_error:nn { ctuthesis } { ctuprocess-twice } }
\DeclareDocumentCommand \ctuprocess@check { } { \msg_error:nn { ctuthesis } { ctuprocess-none } }
\AtBeginDocument{\ctuprocess@check}

\input{ctuth-names.tex}





\endinput
