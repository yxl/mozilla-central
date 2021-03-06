function _mach()
{
  local cur cmds c subcommand mach
  COMPREPLY=()

  # Load the list of commands
  mach=`which mach || echo ./mach`
  cmds=`$mach mach-commands`

  # Look for the subcommand.
  cur="${COMP_WORDS[COMP_CWORD]}"
  subcommand=""
  c=1
  while [ $c -lt $COMP_CWORD ]; do
    word="${COMP_WORDS[c]}"
    for cmd in $cmds; do
      if [ "$cmd" = "$word" ]; then
        subcommand="$word"
      fi
    done
    c=$((++c))
  done

  if [[ "$subcommand" == "help" || -z "$subcommand" ]]; then
      COMPREPLY=( $(compgen -W "$cmds" -- ${cur}) )
  fi

  return 0
}
complete -o default -F _mach mach
