


// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- Flags & options -- Call these routines to enable/disable the following options
// ---------------------------------------------------------------------------------------------------------------------------------

bool	&m_alwaysValidateAll();

// ---------------------------------------------------------------------------------------------------------------------------------

bool	&m_alwaysLogAll();

// ---------------------------------------------------------------------------------------------------------------------------------

bool	&m_alwaysWipeAll();

// ---------------------------------------------------------------------------------------------------------------------------------

bool	&m_randomeWipe();

// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- When tracking down a difficult bug, use this routine to force a breakpoint on a specific allocation count
// ---------------------------------------------------------------------------------------------------------------------------------

void	m_breakOnAllocation(unsigned int count);
