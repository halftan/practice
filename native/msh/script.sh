echo Case 1
echo should echo false
if [ -d /11 ]
then
  echo true
else
  echo false
fi

echo
echo Case 2
echo should echo true
if [ -d . ]
then
  echo true
else
  echo false
fi
